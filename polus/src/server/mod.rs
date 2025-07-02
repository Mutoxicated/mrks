use std::net::TcpListener;

use crate::server::{error::{LSPError, INTERNAL_ERROR}, stream_handler::StreamHandler};

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



pub struct LSP {
    
}

impl LSP {
    pub fn new() ->  Self {
        Self {

        }
    }

    pub fn run(&self) {
        let socket = TcpListener::bind(ADDRESS!()).expect("Binding failed");
        println!("Binding to {} successful! Wait for a client to connect...", ADDRESS!());
        let stream = socket.accept().expect("Failed to establish connection.");
        
        println!("Accepted connection from {}", stream.1);
        let mut stream = stream.0;
        let mut stream_handler = StreamHandler::new(&mut stream);
        
        loop {
            let res = stream_handler.handle_incoming_stream();
            if let Err(x) = res {
                println!("{x}");
                //stream_handler.err_to_stream(&mut stream, x.into_response_err(INTERNAL_ERROR));
            
                continue;
            }
            let has_content_to_process = res.ok().unwrap(); 
            if !has_content_to_process {
                continue;
            }

            stream_handler.clear_content_contributes();
            
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
}