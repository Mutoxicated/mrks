#[repr(C)]
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum TokenType {
    Rubric, Global, Let, Be, Function, If, Identifier, Str, Number, OpenParen, CloseParen, Dot, Comma, Dollar, Colon, Semicolon, Equal, Less, Greater, Plus, Dash, Slash, Asterisk, Caret, Newline, Identation, Eof, Invalid
}

#[repr(C)]
#[derive(Clone, Copy)]
pub struct Range {
    pub min:libc::c_int,
    pub max:libc::c_int
}

#[repr(C)]
#[derive(Clone, Copy)]
pub struct TokenLocation {
    pub column_range: Range,
    pub line: libc::c_int    
}

#[repr(C)]
#[derive(Clone, Copy)]
pub struct Token {
    pub r#type: TokenType,
    pub lexeme: *mut libc::c_char,
    pub location: TokenLocation
}

impl Token {
    pub fn into_rust(&self) -> RustToken {
        RustToken { r#type: self.r#type, location: self.location.clone() }
    } 
}

pub struct RustToken {
    pub r#type: TokenType,
    pub location: TokenLocation,
}

impl Token {

}

#[repr(C)]
pub struct Tokens {
    pub array: *mut Token,
    pub length: libc::c_int
}