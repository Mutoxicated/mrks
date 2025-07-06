mod lexer;
mod server;
mod utils;

use std::net::TcpListener;
use crate::{lexer::markus_tokenize, server::LSP};


fn cstr_to_str(cstr:*mut i8) -> String {
    let mut str = String::new();
    let mut i = 0;
    unsafe {
        while *cstr.offset(i) != '\0' as i8 {
            let c = *cstr.offset(i);
            let c = c.unsigned_abs();
            str.push(c as char);
            i += 1;
        }
    }
    str
}

fn main() {
    let tokens = markus_tokenize(std::fs::read_to_string("./test.k").unwrap().as_str());
    for i in 0..tokens.len() {
        let x = tokens[i];
        let prev = if i == 0 { None } else { Some(tokens[i-1]) };
        let data = x.IntoTokenData(prev);
        println!("Token: {:?} at {}-{}:{} (relative: dl: {}, ds: {}, l: {})", 
            x.r#type, 
            x.location.column_range.min, 
            x.location.column_range.max,
            x.location.line,
            data[0],
            data[1],
            data[2]
        );
    };
    println!("Test passed! Initiating tcp binding at {}...", ADDRESS!());
    let socket = TcpListener::bind(ADDRESS!()).expect("Binding failed");
    println!("Binding to {} successful!", ADDRESS!());
    loop {
        let mut lsp = LSP::new();
        let res = lsp.run(&socket);
        if let Err(x) = res {
            if !x {
                println!("Shutting down server...");
                break;
            }
        }
    }
}