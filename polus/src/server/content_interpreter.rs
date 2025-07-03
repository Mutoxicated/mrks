use std::any::TypeId;

use crate::server::{client::InitializeParams, error::{LSPError, LSPErrorKind}, msg::{Message, NotificationMessage, RequestMessage, ResponseMessage}};


pub struct ContentInterpreter {

}

impl ContentInterpreter {
    pub fn action(content:&str) -> Result<Message, LSPError> {
        let res = serde_json::from_str::<serde_json::Value>(content);
        if let Err(x) = res {
            return LSPError::err(LSPErrorKind::InvalidContent, format!("The Content: {}. The internal error: {}", content, x.to_string()));
        }
        let value = res.unwrap();

        let has_method = value.get("method").is_some();
        let has_id = value.get("id").is_some();

        if has_method && has_id {
            let request:RequestMessage = serde_json::from_value(value).unwrap();
            return Ok(Message::Request(request));
        }
        if has_method {
            let notif:NotificationMessage = serde_json::from_value(value).unwrap();
            return Ok(Message::Notification(notif));
        }
        if has_id {
            let response:ResponseMessage = serde_json::from_value(value).unwrap();
            return Ok(Message::Response(response));
        }

        LSPError::err(LSPErrorKind::UnknownContent, format!("The Content: {}", content))
    }
}