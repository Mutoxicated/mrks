use std::{io::{Read, Write}, net::{TcpListener, TcpStream}, ptr::null_mut};
use crate::lexer::{self, tokens::Tokens, tokens::Token};

fn handle_client(mut stream:TcpStream) {
    let mut string = String::new();
    stream.read_to_string(&mut string).expect("Failed to read stream to string");
    let mut tokens = Tokens{
        array: null_mut(),
        length: 0
    };
    lexer::Tokenize_S(string.as_str(), &mut tokens);
    let mut string_sendout = String::new();// YYYYYYYYYY|xx-xx:x
    string_sendout.reserve_exact(18*tokens.length.unsigned_abs() as usize);
    unsafe {
        for i in 0..tokens.length {
            let token = tokens.array.offset(i as isize) as *mut Token;
            string_sendout.push_str(
                format!("{}-{}:{}|{:?}", 
                (*token).location.column_range.min, 
                (*token).location.column_range.max,
                (*token).location.line,
                (*token).r#type).as_str()
            );
        }
    }
    stream.write_all(string_sendout.as_bytes()).expect("Failed to write tokens output data");
}

pub fn run() {
    let res = TcpListener::bind("127.0.0.1:30000");
    if let Err(x) = res {
        panic!("Bind error: {x}");
    }
    println!("Binded to localhost:3030");
    let listener = res.unwrap();
    for res in listener.incoming() {
        match res {
            Ok(stream) => {
                std::thread::spawn(|| handle_client(stream));
            }
            Err(x) => {
                panic!("Failed to establish connection: {x}");
            }
        }
    }
}