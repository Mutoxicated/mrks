use std::{io::{Error, Read, Write}, net::{TcpListener, TcpStream}, num::ParseIntError, sync::mpsc::{Receiver, Sender}};
use serde::{Deserialize, Serialize};
use serde_json::json;

use crate::utils::consumer::Consumer;

mod lsperror_codes {
    pub static PARSE_ERROR:i32 = -32700;
    pub static INVALID_REQUEST:i32 = -32600;
    pub static METHOD_NOT_FOUND:i32 = -32601;
    pub static INVALID_PARAMS:i32 = -32602;
    pub static INTERNAL_ERROR:i32 = -32603;

    pub static JSONRPC_RESERVED_ERROR_RANGE_START:i32 = -32099;
    pub static SERVER_NOT_INITIALIZED:i32 = -32002;

    pub static REQUEST_FAILED:i32 = -32803;
    pub static CONTENT_MODIFIED:i32 = -32801;
    pub static REQUEST_CANCELLED:i32 = -32800;
}

#[derive(Deserialize, Serialize)]
struct ResponseError {
    code: i32,
    msg: String
}

pub enum LSPError {
    StreamHandlingFailed
}

impl LSPError {
    pub fn into_response_err(&self, context: String) -> ResponseError {
        use lsperror_codes::*;
        match self {
            Self::StreamHandlingFailed => ResponseError { 
                code: INTERNAL_ERROR, 
                msg: format!("Failed to handle stream data. {context}")
            }
        }
    }
}

#[macro_export]
macro_rules! ADDRESS {
    () => {
        "127.0.0.1:30000"
    };
}

macro_rules! MESSAGE_CAP {
    () => {
        300
    };
}

struct StreamHandler<'a> {
    stream: &'a mut TcpStream,
    content:String,
    next_content_length:Option<usize>
}

impl<'a> StreamHandler<'a> {
    fn new(stream: &'a mut TcpStream) -> Self {
        StreamHandler { 
            stream: stream,
            content: String::new(),
            next_content_length: None
        }
    }
    
    fn handle_content_length(&mut self) -> Result<(), ResponseError>{
        if self.content.contains("Content-Length:") {
            let mut num_string = Consumer::take_until(&self.content, 15, '\r' as u8);
            num_string = num_string.trim().to_owned();
            
            let num:Result<usize, ParseIntError> = num_string.parse();
            if let Err(x) = num {
                return Err(LSPError::StreamHandlingFailed.into_response_err(format!("The data: '{}'. The internal error message: '{}'", self.content, x.to_string())));
            }
            self.next_content_length = Some(num.unwrap());
        }

        Ok(())
    }
    
    fn handle_incoming_stream(&mut self) -> Result<(), Error> {
        loop {
            let mut received: Vec<u8> = vec![];
            let mut rx_bytes: Vec<u8> = match self.next_content_length {
                Some(len) => Vec::<u8>::with_capacity(len),
                None => Vec::with_capacity(MESSAGE_CAP!())
            };
            rx_bytes.resize(rx_bytes.capacity(), 0u8);
            
            let bytes_read = self.stream.read(&mut rx_bytes)?;
            received.extend_from_slice(&rx_bytes[..bytes_read]);
            
            if self.next_content_length.is_some() {
                self.next_content_length = None;
            }
            
            self.content = String::from_utf8_lossy(&received).into_owned();
            if self.content.is_empty() {
                continue;
            }

            let res = self.handle_content_length();
            if let Err(response_err) = res {
                self.stream.write_all(serde_json::to_string(&response_err)?.as_bytes())?;
                self.stream.flush()?;
            }
            println!("Got {}", self.content);
            self.content.clear();
        }
        
        Ok(())
    }
}

pub fn run() -> Result<(), Error> {
    let socket = TcpListener::bind(ADDRESS!())?;
    println!("Binding to {} successful! Wait for a client to connect...", ADDRESS!());
    let stream = socket.accept()?;
    
    println!("Accepted connection from {}", stream.1);
    let mut stream = stream.0;
    let mut stream_handler = StreamHandler::new(&mut stream);
    
    stream_handler.handle_incoming_stream() 
}