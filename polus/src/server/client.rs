use serde::{Deserialize, Deserializer};
use crate::server::client_capabilities::*;
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
    pub proccess_id: Option<i32>,
    pub client_info: Option<ClientInfo>,
    #[serde(deserialize_with = "deser_msg_string")]
    pub locale: String,
    
    #[serde(deserialize_with = "deser_msg_string")]
    pub root_path: String,
    #[serde(rename = "rootUri")]
    #[serde(deserialize_with = "deser_msg_string")]
    pub root_uri: DocumentUri,
    #[serde(rename = "initializationOptions")]
    pub initialization_options: serde_json::Value,
    pub capabilities:  ClientCapabilities,
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
    pub text_document: TextDocumentCapabilities

}

#[derive(Deserialize)]
pub struct ClientCapabilities {
    pub workspace: Workspace
}

#[derive(Deserialize)]
pub struct TextDocumentItem {
    pub uri: DocumentUri,
    #[serde(rename = "languageId")]
    pub language_id: String,
    pub version: i32,
    pub text: String
}

#[derive(Deserialize)]
pub struct DidOpenTextDocumentParams {
    #[serde(rename = "textDocument")]
    pub text_document: TextDocumentItem
}

#[derive(Deserialize)]
pub struct VersionedDocumentIdentifier {
    pub version: i32,
    pub uri:DocumentUri
}

#[derive(Deserialize)]
pub struct Position {
    pub line: usize,
    pub character:usize
}

#[derive(Deserialize)]
pub struct Range {
    /// zero based
    pub start: Position,
    /// zero based
    pub end: Position
}

#[derive(Deserialize)]
pub struct TextDocumentChange {
    pub range: Option<Range>,
    pub text: String
}

#[derive(Deserialize)]
pub struct DidChangeTextDocumentParams {
    #[serde(rename = "textDocument")]
    pub versioned_doc_ident: VersionedDocumentIdentifier,
    #[serde(rename = "contentChanges")]
    pub content_changes: Vec<TextDocumentChange>
}

#[derive(Deserialize)]
pub struct DidCloseTextDocumentParams {
    #[serde(rename = "textDocument")]
    pub text_document: TextDocumentIdentifier
}

pub type ProgressToken = String;

#[derive(Deserialize)]
pub struct WorkDoneProgressParams {
    #[serde(rename = "workDoneToken")]
    work_done_token: Option<ProgressToken>,
    #[serde(rename = "partialResultToken")]
    partial_result_token: Option<ProgressToken>,
    #[serde(rename = "textDocument")]
    text_document: DocumentUri,
}

#[derive(Deserialize)]
pub struct TextDocumentIdentifier {
    pub uri: DocumentUri
}

#[derive(Deserialize)]
pub struct SemanticTokensParams {
    #[serde(rename = "workDoneToken")]
    pub work_done_token: Option<ProgressToken>,
    #[serde(rename = "partialResultToken")]
    pub partial_result_token: Option<ProgressToken>,
    #[serde(rename = "textDocument")]
    pub text_document: TextDocumentIdentifier
}
