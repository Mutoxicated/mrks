use std::{net::TcpListener, sync::{Arc, Condvar, Mutex, MutexGuard}, thread, vec};

use crate::{lexer::markus_tokenize, server::{capabilities::{semantic_tokens::{TOKEN_MODIFIERS, TOKEN_TYPES}, SemanticTokens, SemanticTokensLegend, SemanticTokensOptions, ServerCapabilities, ServerInfo, TextDocumentSyncOptions}, client::{DidChangeTextDocumentParams, DidCloseTextDocumentParams, DidOpenTextDocumentParams, SemanticTokensParams}, content_interpreter::ContentInterpreter, error::{LSPError, LSPErrorKind}, msg::{InitializeResult, Message, NotificationMessage, RequestMessage}, stream_handler::StreamHandler}, utils::consumer::Consumer, TEXT_DOC_SYNC_FULL};

mod content_interpreter;
mod error;
mod msg;
mod msg_handler;
mod client_capabilities;
mod client;
mod types;
mod stream_handler;
pub mod capabilities;

#[macro_export]
macro_rules! ADDRESS {
    () => {
        "127.0.0.1:30000"
    };
}

pub struct LSP {
    text_documents_content: Vec<String>,
    text_documents_uri: Vec<String>,
}

impl LSP {
    pub fn new() ->  Self {
        Self {
            text_documents_content: Vec::new(),
            text_documents_uri: Vec::new(),
        }
    }

    pub fn run(&mut self, socket: &TcpListener) -> Result<(), bool> {
        println!("Wait for a client to connect...");
        let stream = socket.accept().expect("Failed to establish connection.");
        println!("Accepted connection from {}", stream.1);
        
        let mut stream_handler = StreamHandler::new(stream.0);

        loop {
            let res = stream_handler.handle_incoming_stream();
            if let Err(x) = &res && x.kind == LSPErrorKind::StreamWasClosed {
                println!("Stream was closed");
                return Err(true);
            }
            if let Err(x) = res {
                if x.kind == LSPErrorKind::StreamWasClosed {
                    return Err(true);
                }
                println!("{x}");
                stream_handler.err_to_stream(x.into_response_err());
                continue;
            }
            let data = res.ok().unwrap();
            if data.is_empty() {
                continue;
            }
            println!("Got {}", data);

            let res = ContentInterpreter::action(data.as_str());
            if let Err(x) = res {
                println!("{x}");
                stream_handler.err_to_stream(x.into_response_err());
                continue;
            }
            let msg = res.ok().unwrap();
            let res = self.handle_message(&mut stream_handler, &msg);
            if let Err(x) = res {
                if x.kind == LSPErrorKind::StreamWasClosed {
                    println!("Stream was shut down");
                    return Err(false)
                }
                println!("{x}");
                stream_handler.err_to_stream(x.into_response_err());
            }
        }
    }

    pub fn handle_message(&mut self, stream_handler:&mut StreamHandler, msg:&Message) -> Result<(), LSPError> {
        match msg {
            msg::Message::Request(x) => {
                println!("Got request: {}", x.method);
                return self.handle_request(stream_handler, x);
            }
            msg::Message::Response(x) => {
                println!("Got response: {}", serde_json::to_string(&x).unwrap());
                Ok(())
            }
            msg::Message::Notification(x) => {
                println!("Got notification: {}", x.method);
                return self.handle_notification(x);
            }
        }
    }

    pub fn get_extension(uri: &str) -> String {
        Consumer::new(-1).take_until_exclude(uri, uri.len()-1, vec!['.'])
    }

    pub fn handle_request(&mut self, stream_handler:&mut StreamHandler, req: &RequestMessage) -> Result<(), LSPError> {
        match req.method.as_str() {
            "initialize" => {
                let legend = SemanticTokensLegend {
                    token_types: TOKEN_TYPES.clone(),
                    token_modifiers: TOKEN_MODIFIERS.clone()
                };
                let semantic_tokens_options = SemanticTokensOptions {
                    semantic_tokens_legend: legend,
                    range: false,
                    full: true,
                };
                let text_document_sync_options = TextDocumentSyncOptions {
                    open_close: true,
                    change: TEXT_DOC_SYNC_FULL!()
                };
                let server_capabilities = ServerCapabilities {
                    semantic_tokens_provider: semantic_tokens_options,
                    text_document_sync: text_document_sync_options
                };
                let initialize_result = InitializeResult {
                    capabilities: server_capabilities,
                    server_info: ServerInfo {
                        name: String::from("polus"),
                        version: String::from("0.1.0")
                    }
                };
                stream_handler.res_to_stream(serde_json::to_value(&initialize_result).unwrap(), req.id);
                Ok(())
            }
            "textDocument/semanticTokens/full" => {
                let params: SemanticTokensParams = serde_json::from_value(req.params.clone().unwrap()).unwrap();

                let mut semantic_tokens = SemanticTokens {
                    result_id: None,
                    data: Vec::new()
                };

                if LSP::get_extension(params.text_document.uri.as_str()).as_str() != "k" {
                    stream_handler.res_to_stream(serde_json::to_value(semantic_tokens).unwrap(), req.id);
                    return Ok(())
                }
                let res = self.text_documents_uri.iter().position(|uri| **uri == params.text_document.uri);
                if res.is_none() {
                    stream_handler.res_to_stream(serde_json::to_value(semantic_tokens).unwrap(), req.id);
                    return Ok(())
                }
                let index = res.unwrap();
                let text_document_content = self.text_documents_content[index].as_str();
                let tokens = markus_tokenize(text_document_content);
                let tokens_len = tokens.len();
                if tokens_len == 1 {
                    semantic_tokens.data.extend_from_slice(&tokens[0].IntoTokenData(None));
                }else if tokens_len != 0 {
                    semantic_tokens.data.extend_from_slice(&tokens[0].IntoTokenData(None));
                    for i in 1..tokens.len() {
                        let previous = tokens[i-1];
                        semantic_tokens.data.extend_from_slice(&tokens[i].IntoTokenData(Some(previous)));
                    }
                }
                stream_handler.res_to_stream(serde_json::to_value(semantic_tokens).unwrap(), req.id);
                Ok(())
            }
            "shutdown" => LSPError::err(LSPErrorKind::StreamWasClosed, String::new()),
            _ => LSPError::err(LSPErrorKind::UnknownRequestMethod, format!("The method name: {}", req.method))
        }
    }

    pub fn handle_notification(&mut self, notif: &NotificationMessage) -> Result<(), LSPError> {
        match notif.method.as_str() {
            "textDocument/didOpen" => {
                let params: DidOpenTextDocumentParams = serde_json::from_value(notif.params.clone()).unwrap();

                if LSP::get_extension(params.text_document.uri.as_str()).as_str() != "k" {
                    return Ok(())
                }

                self.text_documents_content.push(params.text_document.text);
                self.text_documents_uri.push(params.text_document.uri);
                Ok(())
            }
            "textDocument/didChange" => {
                let params: DidChangeTextDocumentParams = serde_json::from_value(notif.params.clone()).unwrap();

                let uri = params.versioned_doc_ident.uri;
                if LSP::get_extension(uri.as_str()).as_str() != "k" {
                    return Ok(())
                }

                let doc_index = self.text_documents_uri.iter().position(|x| **x == uri).unwrap();
                let mut text = self.text_documents_content[doc_index].as_str().as_bytes().to_vec();
                
                let changes = params.content_changes;
                for i in 0..changes.len() {
                    let change = &changes[i];

                    if change.range.is_none() {
                        text = change.text.as_str().as_bytes().to_vec();
                        continue;
                    }
                    let range = change.range.as_ref().unwrap();
                    
                    let mut line_counter = 0usize;
                    let mut column_counter = 0usize;
                    let (mut start_index, mut end_index) = (0usize, 0usize);
                    for j in 0..text.len() {
                        column_counter += 1;
                        if text[j] == '\n' as u8 {
                            column_counter = 0;
                            line_counter += 1;
                            if line_counter == range.start.line && column_counter == range.start.character {
                                start_index = j;
                            }
                            if line_counter == range.end.line && column_counter == range.end.character {
                                end_index = j;
                            }
                        }
                    }
                    //text[0..start_index]+change.text+text[end_index..text.len()]
                    let mut slice = text[0..start_index].to_vec().clone();
                    slice.extend_from_slice(change.text.as_bytes());
                    slice.extend_from_slice(&text[end_index..text.len()]);
                    text = slice;
                }

                self.text_documents_content[doc_index] = String::from_utf8(text).unwrap();
                
                Ok(())
            }
            "textDocument/didClose" => {
                let params: DidCloseTextDocumentParams = serde_json::from_value(notif.params.clone()).unwrap();

                if LSP::get_extension(params.text_document.uri.as_str()).as_str() != "k" {
                    return Ok(())
                }

                let index = self.text_documents_uri.iter().position(|uri| **uri == params.text_document.uri).unwrap();
                self.text_documents_content.remove(index);
                self.text_documents_uri.remove(index);
                
                Ok(())
            }
            "initialized" => Ok(()),
            _ => Ok(())
        }//LSPError::err(LSPErrorKind::UnknownNotificationMethod, format!("The method name: {}", notif.method))
    }
}