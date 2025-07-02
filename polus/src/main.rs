mod lexer;
mod server;
mod utils;

use std::{ptr::null_mut};
use lexer::tokens::{Tokens, Token};
use lexer::{Tokenize_S};

use crate::lexer::tokens::RustToken;
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
    println!("Running test...");
    let mut tkns:Vec<RustToken> = Vec::new();
    let mut tokens = Tokens{
        array: null_mut() as *mut Token,
        length: 0
    };
    let file = std::fs::read_to_string("./test.k").expect("Failed to read ./test.k");
    Tokenize_S(file.as_str(), &mut tokens);
    tkns.reserve_exact(tokens.length.unsigned_abs() as usize);

    unsafe {
        for i in 0..tokens.length {
            let token:*const Token = tokens.array.offset(i as isize) as *const Token;
            let str = cstr_to_str((*token).lexeme);
            println!("Token {i}: {:?} at {}-{}:{}: \"{}\"", (*token).r#type, (*token).location.column_range.min, (*token).location.column_range.max, (*token).location.line, str.as_str());
            
            tkns.push((*token).into_rust());
        }
    }

    println!("Test passed! Initiating tcp binding at {}...", ADDRESS!());

    let lsp = LSP::new();
    lsp.run();
}