use std::{io::{BufRead, BufReader, Error, Read, Write}, net::{TcpListener, TcpStream}, num::ParseIntError};
use crate::{server::{error::{LSPError, LSPErrorKind, ResponseError}, msg::ResponseMessage, LSP}, utils::consumer::Consumer};

#[derive(PartialEq, Eq)]
pub enum ContentKind {
    JsonRpc,
    Invalid
}

impl From<&str> for ContentKind {
    fn from(value: &str) -> Self {
        if value.contains("jsonrpc") { ContentKind::JsonRpc }
        else { ContentKind::Invalid }
    }
}

#[derive(PartialEq, Eq)]
pub enum Charset {
    Utf8,
    Invalid
}

impl From<&str> for Charset {
    fn from(value: &str) -> Self {
        match value {
            "utf-8" => Charset::Utf8,
            _ => Charset::Invalid 
        }
    }
}

pub struct ContentType {
    kind: ContentKind,
    charset: Charset
}

pub type StreamResult = Result<String, LSPError>;

pub struct StreamHandler {
    stream: TcpStream,
    content_length:Option<usize>,
    content_type: Option<ContentType>,
    pub results: Vec<StreamResult>
}

macro_rules! MESSAGE_CAP {
    () => {
        200
    };
}

impl StreamHandler {
    pub fn new(stream: TcpStream) -> Self {
        StreamHandler { 
            stream,
            content_length: None,
            content_type: None,
            results: Vec::new()
        }
    }

    fn handle_content_header(&mut self, data:&str) -> Result<(), LSPError>{
        let mut num_string = Consumer::default().take_until(data, 15, ['\n'].to_vec());
        num_string = num_string.trim().to_owned();
        
        let num:Result<usize, ParseIntError> = num_string.parse();
        if let Err(x) = num {
            return LSPError::err(LSPErrorKind::StreamHandlingFailed, x.to_string())
        }
        self.content_length = Some(num.unwrap());

        let length = data.len();
        let after_content_length = 20+num_string.len();
        if length <= after_content_length {
            self.content_type = Some(ContentType {
                kind: ContentKind::JsonRpc,
                charset: Charset::Utf8,
            });
            return Ok(())
        }
        let content_type_data= &data[after_content_length..length];
        if !content_type_data.contains("Content-Type") {
            return LSPError::err(LSPErrorKind::InvalidContentHeader, format!("The content: '{}'. Parsed through '{}'. Content type data was invalid.", data, content_type_data));
        }
        let kind = Consumer::default().take_until_exclude(content_type_data, 13, [';'].to_vec());
        let charset = Consumer::default().take_until_exclude(content_type_data , 13+kind.len(), ['\r'].to_vec());
        
        let kind = kind.trim();// "Content-Length: 5918\r\n\r\n!"
        let charset = charset.trim();

        self.content_type = Some(ContentType {
            kind: ContentKind::from(kind),
            charset: Charset::from(charset)
        });
        // TODO: Validate content_type input

        Ok(())
    }
    
    /// the `bool` in `Result` tells the lsp whether it has received content that should be processed by it
    pub fn handle_incoming_stream(&mut self) {
        let mut received: Vec<u8> = vec![];
        let mut rx_bytes: Vec<u8> = Vec::new();
        if self.content_length.is_some() {
            rx_bytes.resize(self.content_length.unwrap(), 0u8);
        }else {
            rx_bytes.resize(MESSAGE_CAP!(), 0u8);
        }
        
        let res = self.stream.read(&mut rx_bytes);
        if let Err(x) = res {
            self.results.push(LSPError::err(LSPErrorKind::StreamHandlingFailed, x.to_string()));
            return
        }
        let bytes_read = res.unwrap();
        if bytes_read == 0 {
            self.results.push(LSPError::err(LSPErrorKind::StreamWasClosed, String::new()));
            return
        }
        received.extend_from_slice(&rx_bytes[..bytes_read]);
        
        let data = String::from_utf8_lossy(&received).into_owned();
        if data.is_empty() {
            self.content_length = None;
            self.results.push(Ok(String::new()));
            return
        }
        if data.contains("Content-Length:") {
            println!("Got {}", data);
            let res = self.handle_content_header(data.as_str());
            if let Err(x) = res {
                self.results.push(Err(x));
                return
            }
            self.results.push(Ok(String::new()));
            return
        }
        self.content_length = None;

        println!("Got {}", data);
        self.results.push(Ok(data.to_owned()));
    }
    
    pub fn err_to_stream(&mut self, response_err:ResponseError) {
        let string = serde_json::to_string(&response_err).unwrap();
        println!("[Err] Send {}", string);
        let _ = self.stream.write_all(format!("[Err] Send {}", string.len()).as_bytes());
        let _ = self.stream.write_all(string.as_bytes());
    }

    pub fn res_to_stream(&mut self, result: serde_json::Value, id:i32) {
        let response = ResponseMessage {
            jsonrpc: String::from("2.0"),
            id,
            result: Some(result),
            error: None
        };
        let response_json = serde_json::to_string(&response).unwrap();

        let o = format!("Content-Length: {}\r\n\r\n", response_json.len());
        println!("Send {o}");
        let _ = self.stream.write_all(o.as_bytes());
        println!("Send {response_json}");
        let _ = self.stream.write_all(response_json.as_bytes());
    }
}

