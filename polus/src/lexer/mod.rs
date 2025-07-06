use std::ptr::null_mut;

use crate::lexer::tokens::{RustToken, Token, Tokens};

pub mod tokens;

#[link(name = "lexer")]
unsafe extern "C" {
    fn Tokenize(file: *const libc::c_char, tokens: *mut tokens::Tokens);
}

pub fn markus_tokenize(file: &str) -> Vec<RustToken> {
    let bytes = file.as_bytes();
    let mut ibytes:Vec<libc::c_char> = Vec::new();
    for v in bytes {
        ibytes.push(*v as i8);
    }
    if ibytes.len() == 0 {
        ibytes.push('\0' as i8);
    }

    let mut tokens: Tokens = Tokens {
        array: null_mut() as *mut Token,
        length: 0,
    };
    unsafe {
        Tokenize(ibytes.as_ptr(), &mut tokens);

        let mut rust_tokens = Vec::new();
        rust_tokens.reserve_exact(tokens.length as usize);
        for i in 0..tokens.length {
            rust_tokens.push((tokens.array.offset(i as isize) as *const Token).as_ref().unwrap().into_rust());
        }

        return rust_tokens;
    };
}