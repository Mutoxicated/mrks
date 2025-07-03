use std::{net::{TcpListener, TcpStream}, vec};

use crate::server::{content_interpreter::ContentInterpreter, error::{LSPError, LSPErrorKind, INTERNAL_ERROR}, msg::{InitializeResult, Message, RequestMessage, ResponseMessage, SemanticTokensLegend, SemanticTokensOptions, ServerCapabilities, ServerInfo}, stream_handler::StreamHandler};

mod content_interpreter;
mod error;
mod msg;
mod msg_handler;
mod capabilities;
mod client;
mod types;
mod stream_handler;

#[macro_export]
macro_rules! ADDRESS {
    () => {
        "127.0.0.1:30000"
    };
}



pub struct LSP<'a> {
    socket: &'a TcpListener,
    stream_handler: StreamHandler
}

impl<'a> LSP<'a> {
    pub fn connect(socket: &'a TcpListener) ->  Self {
        println!("Wait for a client to connect...");
        let stream = socket.accept().expect("Failed to establish connection.");
        println!("Accepted connection from {}", stream.1);
        Self {
            socket,
            stream_handler: StreamHandler::new(stream.0)
        }
    }

    pub fn run(&mut self) -> Result<(), bool> {
        loop {
            let res = self.stream_handler.handle_incoming_stream();
            if let Err(x) = res {
                if x.kind == LSPErrorKind::StreamWasClosed {
                    return Err(true);
                }
                println!("{x}");
                self.stream_handler.err_to_stream(x.into_response_err());
                continue;
            }
            let has_content_to_process = res.ok().unwrap(); 
            if !has_content_to_process {
                continue;
            }

            let res = ContentInterpreter::action(self.stream_handler.data.as_str());
            if let Err(x) = res {
                println!("{x}");
                self.stream_handler.err_to_stream(x.into_response_err());
                continue;
            }
            let msg = res.ok().unwrap();
            let res = self.handle_message(&msg);
            if let Err(x) = res {
                if x.kind == LSPErrorKind::StreamWasClosed {
                    return Err(false)
                }
                println!("{x}");
                self.stream_handler.err_to_stream(x.into_response_err());
            }
            
            /* The main loop
            
            // handle stream input (StreamHandler) --- DONE ---
            //-> handles basic input like 'Content-Length' and 'Content-Type'
            
            // interpret bigger stream input -> output stuff understandable to lsp (ContentInterpreter)
            //-> handles actual request/response/notification jsonrpc messages
            //-> outputs information that the lsp can utilize to convert jsonrpc messages to their struct counterpart
            
            // lsp logic (LSPCore)
            
            */
        }
    }

    pub fn handle_message(&mut self, msg:&Message) -> Result<(), LSPError> {
        match msg {
            msg::Message::Request(x) => {
                println!("Got request: {}", x.method);
                return self.handle_request(x);
            }
            msg::Message::Response(x) => {
                println!("Got response: {}", serde_json::to_string(&x).unwrap());
                Ok(())
            }
            msg::Message::Notification(x) => {
                println!("Got notification: {}", x.method);
                Ok(())
            }
        }
    }

    pub fn handle_request(&mut self, req: &RequestMessage) -> Result<(), LSPError> {
        match req.method.as_str() {
            "initialize" => {
                let legend = SemanticTokensLegend {
                token_types: vec!["variable".to_owned(), "keyword".to_owned()],
                token_modifiers: vec!["static".to_owned()]

                };
                let semantic_tokens_options = SemanticTokensOptions {
                    semantic_tokens_legend: legend,
                    range: false,
                    full: true,
                };
                let server_capabilities = ServerCapabilities {
                    semantic_tokens_options
                };
                let initialize_result = InitializeResult {
                    capabilities: server_capabilities,
                    server_info: ServerInfo {
                        name: String::from("polus"),
                        version: String::from("0.1.0")
                    }
                };
                self.stream_handler.res_to_stream(serde_json::to_value(&initialize_result).unwrap(), req.id);
                Ok(())
            }
            "shutdown" => LSPError::err(LSPErrorKind::StreamWasClosed, String::new()),
            _ => LSPError::err(LSPErrorKind::UnknownRequestMethod, format!("The method name: {}", req.method))
        }
    }
}