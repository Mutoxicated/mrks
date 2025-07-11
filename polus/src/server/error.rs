use serde::{Deserialize, Serialize};

#[macro_export] macro_rules! PARSE_ERROR { () => { -32700 } }
#[macro_export] macro_rules! INVALID_REQUEST { () => { -32600 } }
#[macro_export] macro_rules! METHOD_NOT_FOUND { () => { -32601 } }
#[macro_export] macro_rules! INVALID_PARAMS { () => { -32602 } }
#[macro_export] macro_rules! INTERNAL_ERROR { () => { -32603 } }

#[macro_export] macro_rules! SERVER_NOT_INITIALIZED { () => { -32002 } }

#[macro_export] macro_rules! REQUEST_FAILED { () => { -32803 } }
#[macro_export] macro_rules! CONTENT_MODIFIED { () => { -32801 } }
#[macro_export] macro_rules! REQUEST_CANCELLED { () => { -32800 } }

#[derive(Deserialize, Serialize)]
pub struct ResponseError {
    code: i32,
    msg: String
}

#[derive(Clone)]
pub struct LSPError {
    pub kind: LSPErrorKind,
    context: String,
}

impl std::fmt::Display for LSPError {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.write_str(format!("{}. Context: {}", self.kind.to_string(), self.context).as_str())
    }
}

impl LSPError {
    pub fn new(kind: LSPErrorKind, context: String) -> Self {
        Self {
            kind,
            context
        }
    }

    pub fn err<T>(kind: LSPErrorKind, context: String) -> Result<T, Self> {
        Err(LSPError::new(kind, context))
    }

    pub fn into_response_err(&self) -> ResponseError {
        ResponseError {
            code: self.kind.get_code(),
            msg: format!("{}. Context: {}", self.kind.to_string(), self.context)
        }
    }
}

#[derive(PartialEq, Eq, Clone, Copy)]
pub enum LSPErrorKind {
    StreamHandlingFailed,
    InvalidContentHeader,
    InvalidContent,
    UnknownContent,
    StreamWasClosed,
    UnknownRequestMethod,
    UnknownNotificationMethod
}

impl LSPErrorKind {
    pub fn to_string(&self) -> String {
        match self {
            Self::StreamHandlingFailed => "Failed to handle stream data".to_owned(),
            Self::InvalidContentHeader => "The content header of the stream was invalid".to_owned(),
            Self::InvalidContent => "The content of the stream could not be parsed into a json value".to_owned(),
            Self::UnknownContent => "The content of the stream was exotic".to_owned(),
            Self::StreamWasClosed => "The tcp stream was closed".to_owned(),
            Self::UnknownRequestMethod => "The given request method was exotic".to_owned(),
            Self::UnknownNotificationMethod => "The given notification method was exotic".to_owned()
        }
    }

    pub fn get_code(&self) -> i32 {
        match self {
            Self::StreamHandlingFailed => INTERNAL_ERROR!(),
            Self::InvalidContentHeader => PARSE_ERROR!(),
            Self::InvalidContent => PARSE_ERROR!(),
            Self::UnknownContent => PARSE_ERROR!(),
            Self::StreamWasClosed => INTERNAL_ERROR!(),
            Self::UnknownRequestMethod => INVALID_REQUEST!(),
            Self::UnknownNotificationMethod => METHOD_NOT_FOUND!()
        }
    }
}
