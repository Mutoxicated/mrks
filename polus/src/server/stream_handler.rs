use std::{io::{BufRead, BufReader, Read, Write}, net::{TcpListener, TcpStream}, num::ParseIntError};
use crate::{server::{error::{LSPError, LSPErrorKind, ResponseError}, LSP}, utils::consumer::Consumer};

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

pub struct StreamHandler<'a> {
    stream: &'a mut TcpStream,
    data:String,
    content_length:Option<usize>,
    content_type: Option<ContentType>,
}

macro_rules! MESSAGE_CAP {
    () => {
        200
    };
}

impl<'a> StreamHandler<'a> {
    pub fn new(stream: &'a mut TcpStream) -> Self {
        StreamHandler { 
            stream,
            data: String::new(),
            content_length: None,
            content_type: None,
        }
    }

    pub fn clear_content_contributes(&mut self) {
        self.content_length = None;
        self.content_type = None;
    }
    
    fn handle_content_header(&mut self) -> Result<(), LSPError>{
        let mut num_string = Consumer::take_until(&self.data, 15, ['\r'].to_vec());
        num_string = num_string.trim().to_owned();
        
        let num:Result<usize, ParseIntError> = num_string.parse();
        if let Err(x) = num {
            return LSPError::err(LSPErrorKind::StreamHandlingFailed, x.to_string())
        }
        self.content_length = Some(num.unwrap());
        // let kind = Consumer::take_until(&self.data, 13+num_string.len()+2, [';'].to_vec());
        // let charset = Consumer::take_until(&self.data , 13+kind.len(), ['\r'].to_vec());
        
        // let kind = kind.trim();
        // let charset = charset.trim();

        // self.content_type = Some(ContentType {
        //     kind: ContentKind::from(kind),
        //     charset: Charset::from(charset)
        // });
        return Ok(())
    }
    
    /// the `bool` in `Result` tells the lsp whether it has received content that should be processed by it
    pub fn handle_incoming_stream(&mut self) -> Result<bool, LSPError> {
        let mut received: Vec<u8> = vec![];
        let mut rx_bytes: Vec<u8> = Vec::new();
        if self.content_length.is_some() {
            rx_bytes.resize(self.content_length.unwrap(), 0u8);
        }else {
            rx_bytes.resize(MESSAGE_CAP!(), 0u8);
        }
        
        let res = self.stream.read(&mut rx_bytes);
        if let Err(x) = res {
            return LSPError::err(LSPErrorKind::StreamHandlingFailed, x.to_string())
        }
        let bytes_read = res.unwrap();
        received.extend_from_slice(&rx_bytes[..bytes_read]);
        
        self.data = String::from_utf8_lossy(&received).into_owned();
        if self.data.is_empty() {
            return Ok(false);
        }
        if self.data.contains("Content-Length:") {
            self.handle_content_header()?;
            println!("Got {}", self.data);
            return Ok(false);
        }
        
        println!("Got {}", self.data);
        Ok(true)
    }
    
    /*
    pub fn err_to_stream(&mut self, response_err:ResponseError) -> Result<(), LSPError> {
        let res = stream.write_all(serde_json::to_string(&response_err).unwrap().as_bytes());
        if let Err(x) = res {
            return LSPError::err(LSPErrorKind::StreamHandlingFailed, x.to_string());
        }

        Ok(())
    }
    */
}

