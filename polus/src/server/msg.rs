use serde::{Deserialize, Deserializer};
use crate::server::error::ResponseError;
use crate::server::capabilities::*;







#[derive(Deserialize)]
pub struct RequestMessage {
    jsonrpc:String,
    id:i32,
    method:String,
    params:serde_json::Value
}

#[derive(Deserialize)]
pub struct ResponseMessage {
    jsonrpc:String,
    id:i32,
    value:serde_json::Value,
    error:Option<ResponseError>
}

#[derive(Deserialize)]
pub struct NotificationMessage {
    jsonrpc:String,
    method:String,
    params:serde_json::Value
}