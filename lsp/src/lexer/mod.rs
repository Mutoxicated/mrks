pub mod tokens;

#[link(name = "lexer")]
unsafe extern "C" {
    fn Tokenize(file: *const libc::c_char, tokens: *mut tokens::Tokens);
}

pub fn Tokenize_S(file: &str, tokens: *mut tokens::Tokens) {
    let bytes = file.as_bytes();
    let mut ibytes:Vec<i8> = Vec::new();
    ibytes.reserve_exact(bytes.len());
    for v in bytes {
        ibytes.push(*v as i8);
    }
    unsafe {
        Tokenize(ibytes.as_ptr(), tokens);
    }
}