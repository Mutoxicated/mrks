mod lexer;
mod server;
mod utils;

use std::net::TcpListener;
use crate::server::LSP;


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