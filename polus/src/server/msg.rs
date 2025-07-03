use std::{io::Write, net::TcpStream};

use serde::{Deserialize, Serialize};
use crate::server::error::ResponseError;

pub enum Message {
    Request(RequestMessage),
    Response(ResponseMessage),
    Notification(NotificationMessage)
}

#[derive(Deserialize)]
pub struct RequestMessage {
    pub jsonrpc:String,
    pub id:i32,
    pub method:String,
    pub params:Option<serde_json::Value>
}
#[derive(Deserialize, Serialize)]
pub struct ResponseMessage {
    pub jsonrpc:String,
    pub id:i32,
    pub result:Option<serde_json::Value>,
    pub error:Option<ResponseError>
}
#[derive(Deserialize, Serialize)]
pub struct NotificationMessage {
    pub jsonrpc:String,
    pub method:String,
    pub params:serde_json::Value
}

//Server-side results sent over to client
#[derive(Serialize)]
pub struct InitializeResult {
    pub capabilities: ServerCapabilities,
    #[serde(rename = "serverInfo")]
    pub server_info: ServerInfo
}

#[derive(Serialize)]
pub struct ServerCapabilities {
    #[serde(rename = "semanticTokensOptions")]
    pub semantic_tokens_options: SemanticTokensOptions
}

#[derive(Serialize)]
pub struct SemanticTokensLegend {
    #[serde(rename = "tokenTypes")]
    pub token_types: Vec<String>,
    #[serde(rename = "tokenModifiers")]
    pub token_modifiers: Vec<String>
}

#[derive(Serialize)]
pub struct SemanticTokensOptions {
    #[serde(rename = "semanticTokensLegend")]
    pub semantic_tokens_legend: SemanticTokensLegend,
    pub range:bool,
    pub full:bool
}

#[derive(Serialize)]
pub struct ServerInfo {
    pub name: String,
    pub version: String
}