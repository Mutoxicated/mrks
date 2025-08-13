use std::ptr::null_mut;

use crate::lexer::tokens::{RustToken, Token, TokenType, Tokens};

pub mod tokens;

#[link(name = "lexer")]
unsafe extern "C" {
    fn Tokenize(file: *const libc::c_char, tokens: *mut tokens::Tokens);
}

/// removes the identation, newline and eof tokens
pub fn markus_tokenize(file: &str) -> Vec<RustToken> {
    let bytes = file.as_bytes();
    let mut ibytes:Vec<libc::c_char> = Vec::new();
    for v in bytes {
        ibytes.push(*v as i8);
    }
    ibytes.push('\0' as i8);

    let mut tokens: Tokens = Tokens {
        array: null_mut() as *mut Token,
        length: 0,
    };
    unsafe {
        Tokenize(ibytes.as_ptr(), &mut tokens);

        let mut rust_tokens = Vec::new();
        rust_tokens.reserve_exact(tokens.length as usize);
        for i in 0..tokens.length {
            let rust_token = (tokens.array.offset(i as isize) as *const Token).as_ref().unwrap().into_rust();
            match rust_token.r#type {
                TokenType::Identation => { continue; },
                TokenType::Newline => { continue; },
                TokenType::Eof => { continue; }
                _ => { rust_tokens.push(rust_token); }
            }
        }

        return rust_tokens;
    };
}