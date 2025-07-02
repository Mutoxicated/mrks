use serde::{Deserialize, Deserializer};
use crate::server::capabilities::*;
use crate::server::types::*;

fn deser_msg_string<'de, D: Deserializer<'de>>(deserializer: D) -> Result<String, D::Error> {
    Option::deserialize(deserializer).map(|s| s.unwrap_or_else(|| "(missing".to_owned()))
}

#[derive(Deserialize)]
pub struct ClientInfo {
    name: String,
    verstion: String
}

#[derive(Deserialize)]
pub struct FileOperations {
    #[serde(default)]
    #[serde(rename = "dynamicRegistration")]
    dynamic_registration: bool,
    

}

#[derive(Deserialize)]
pub struct InitializeParams {
    proccess_id: Option<i32>,
    client_info: Option<ClientInfo>,
    #[serde(deserialize_with = "deser_msg_string")]
    locale: String,
    
    #[serde(deserialize_with = "deser_msg_string")]
    root_path: String,
    #[serde(rename = "rootUri")]
    #[serde(deserialize_with = "deser_msg_string")]
    root_uri: DocumentUri,
    #[serde(rename = "initializationOptions")]
    initialization_options: serde_json::Value,
    capabilities:  ClientCapabilities,
}

#[derive(Deserialize)]
pub struct Workspace {
    #[serde(default)]
    #[serde(rename = "applyEdit")]
    apply_edit: bool,
    #[serde(rename = "workspaceEdit")]
    workspace_edit: WorkspaceEditClientCapabilities,
    #[serde(rename = "didChangeConfiguration")]
    did_change_configuration: DidChangeConfigurationClientCapabilities,
    #[serde(rename = "didChangeWatchedFiles")]
    did_change_watched_files: DidChangeWatchedFilesClientCapabilities,
    symbol: WorkspaceSymbolClientCapabilities,
    #[serde(rename = "executeCommand")]
    execute_command: ExecuteCommandClientCapabilities,
    #[serde(rename = "workspacesFolders")]
    workspace_folders: bool,
    configuration: bool,
    #[serde(rename = "semanticTokens")]
    semantic_tokens: SemanticTokensWorkspaceClientCapabilities,
    #[serde(rename = "codeLens")]
    code_lens: CodeLensWorkspaceClientCapabilities,
    file_operations: FileOperations,
    #[serde(rename = "textDocument")]
    text_document: TextDocumentCapabilities

}

#[derive(Deserialize)]
pub struct ClientCapabilities {
    workspace: Workspace
}
