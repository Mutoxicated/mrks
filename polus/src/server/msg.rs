use serde::{Deserialize, Serialize};
use crate::server::{capabilities::{ServerCapabilities, ServerInfo}, error::ResponseError};

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
