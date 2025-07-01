use std::{io::{Read, Write}, net::{TcpListener, TcpStream}, ptr::null_mut, str::from_utf8};
use crate::lexer::{self, tokens::{Token, TokenType, Tokens}};

fn handle_client(mut stream:TcpStream) {
    let mut bytes:[u8; 1024] = [0; 1024];
    stream.read(&mut bytes).expect("Failed to read bytes from stream");
    let a = String::from_utf8_lossy(&bytes);
    let string = a.trim().chars().as_str();
    let mut tokens = Tokens{
        array: null_mut() as *mut Token,
        length: 0
    };
    lexer::Tokenize_S(string, &mut tokens);
    let mut string_sendout = String::new();// YYYYYYYYYY|xx-xx:x
    string_sendout.reserve_exact(18*tokens.length.unsigned_abs() as usize);
    unsafe {
        for i in 0..tokens.length {
            let token = tokens.array.offset(i as isize) as *mut Token;
            if (*token).r#type == TokenType::Eof || (*token).r#type == TokenType::Newline || (*token).r#type == TokenType::Identation {
                continue;
            }
            string_sendout.push_str(
                format!("{}-{}:{}|{:?}\n", 
                (*token).location.column_range.min, 
                (*token).location.column_range.max,
                (*token).location.line,
                (*token).r#type).as_str()
            );
        }
    }
    println!("String: {}", string_sendout);
    let _ = stream.write_all(string_sendout.as_bytes());
    //stream.write_all(string_sendout.as_bytes()).expect("Failed to write tokens output data");
}

pub fn run() {
    let res = TcpListener::bind("127.0.0.1:30000");
    if let Err(x) = res {
        panic!("Bind error: {x}");
    }
    println!("Binded to localhost:30000");
    let listener = res.unwrap();
    for res in listener.incoming() {
        match res {
            Ok(stream) => {
                println!("Got stream.");
                std::thread::spawn(|| { handle_client(stream) });
            }
            Err(x) => {
                panic!("Failed to establish connection: {x}");
            }
        }
    }
}