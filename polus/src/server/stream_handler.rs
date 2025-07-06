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
    content_length: Option<usize>,
    content_type: Option<ContentType>,
    data: String
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
            data: String::new()
        }
    }

    /// the `bool` in `Result` tells the lsp whether it has received content that should be processed by it
    pub fn handle_incoming_stream(&mut self) -> Result<String, LSPError> {
        let mut received: Vec<u8> = vec![];
        let mut rx_bytes: Vec<u8> = Vec::new();
        if self.content_length.is_some() {
            rx_bytes.resize(self.content_length.unwrap(), 0u8);
        }else {
            rx_bytes.resize(MESSAGE_CAP!(), 0u8);
        }
        
        let res = self.stream.read(&mut rx_bytes);
        if let Err(x) = res {
            return LSPError::err(LSPErrorKind::StreamHandlingFailed, x.to_string());
        }
        let bytes_read = res.unwrap();
        if bytes_read == 0 {
           return LSPError::err(LSPErrorKind::StreamWasClosed, String::new());
        }
        received.extend_from_slice(&rx_bytes[..bytes_read]);
        
        let data = String::from_utf8_lossy(&received).into_owned();
        if data.is_empty() {
            self.content_length = None;
            return Ok(String::new());
        }
        if self.content_length.is_some() {
            self.data.push_str(data.as_str());
            self.content_length = None;
            let str = self.data.clone();
            self.data.clear();
            return Ok(str);
        }

        let content_length = Consumer::default().take_until(data.as_str(), 0, ['\n'].to_vec());
        let num = Consumer::new(-1).take_until_exclude(&content_length, content_length.len()-1, [' '].to_vec());
        let parsed_num:usize = num.parse().unwrap();

        let content_type = Consumer::default().take_until(data.as_str(), content_length.len(), ['\n'].to_vec());
        
        let content_header_length = content_length.len()+content_type.len();
        let data_length = data.len();
        if data_length == content_header_length {
            self.content_length = Some(parsed_num);
            return Ok(String::new());
        }
        self.data = data[content_header_length..data_length].to_string();
        self.content_length = Some(parsed_num-self.data.len());
        println!("Got {}", data);
        Ok(String::new())
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

