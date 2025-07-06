use crate::server::{self, capabilities::semantic_tokens::{TokenModifierLegend, TokenTypeLegend}};

#[repr(C)]
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum TokenType {
    Rubric, Global, Let, Be, Function, If, Identifier, Str, Number, OpenParen, CloseParen, Dot, Comma, Dollar, Colon, Semicolon, Equal, Less, Greater, Plus, Dash, Slash, Asterisk, Caret, Newline, Identation, Eof, Invalid
}

impl TokenType {
    pub fn into_legend(&self) -> (TokenTypeLegend, TokenModifierLegend) {
        use TokenTypeLegend::*;
        use TokenModifierLegend::*;
        match *self {
            Self::Rubric => (Keyword, Class),
            Self::Identifier => (Variable, Class),
            _ => (Variable, Class)
        }
    }
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

#[derive(Clone, Copy)]
pub struct RustToken {
    pub r#type: TokenType,
    pub location: TokenLocation,
}

impl RustToken {
    pub fn IntoTokenData(&self, relative_to:Option<RustToken>) -> Vec<usize> {
        let mut data = Vec::<usize>::new();
        let legend = self.r#type.into_legend();
        if relative_to.is_none() {
            data.extend_from_slice(&[
                self.location.line as usize, 
                self.location.column_range.min as usize, 
                (self.location.column_range.max-self.location.column_range.min) as usize, 
                legend.0 as usize,
                legend.1 as usize
            ]);
            return data
        }
        let relative_to = relative_to.unwrap();
        let start_char = if self.location.line == relative_to.location.line { 
            (self.location.column_range.min-relative_to.location.column_range.max).unsigned_abs() as usize 
        } else {
            self.location.column_range.min as usize
        };
        data.extend_from_slice(&[
            (self.location.line-relative_to.location.line) as usize, 
            start_char, 
            (self.location.column_range.max-self.location.column_range.min).unsigned_abs() as usize, 
            legend.0 as usize,
            legend.1 as usize
        ]);

        data
    }
}

#[repr(C)]
pub struct Tokens {
    pub array: *mut Token,
    pub length: libc::c_int
}

impl Tokens {
    pub fn for_loop<F>(&mut self, mut f: F)
        where
            Self: Sized,
            F: FnMut(RustToken, i32), {
        
        unsafe {
            for i in 0..self.length {
                let token = self.array.offset(i as isize).as_ref().unwrap().into_rust();
                f(token, i);
            }
        }
    }
}