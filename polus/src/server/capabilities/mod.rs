use serde::Serialize;

pub mod semantic_tokens;

#[derive(Serialize)]
pub struct ServerCapabilities {
    #[serde(rename = "semanticTokensProvider")]
    pub semantic_tokens_provider: SemanticTokensOptions,
    #[serde(rename = "textDocumentSync")]
    pub text_document_sync: TextDocumentSyncOptions
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
pub struct SemanticTokens {
    #[serde(rename = "resultId")]
    pub result_id: Option<String>,
    pub data: Vec<usize>
}

#[macro_export]
macro_rules! TEXT_DOC_SYNC_NONE { () => { 0 } }
#[macro_export]
macro_rules! TEXT_DOC_SYNC_FULL { () => { 1 } }
#[macro_export]
macro_rules! TEXT_DOC_SYNC_INCREMENTAL { () => { 2 } }

#[derive(Serialize)]
pub struct TextDocumentSyncOptions {
    #[serde(rename = "openClose")]
    pub open_close:bool,
    pub change:usize,
}

#[derive(Serialize)]
pub struct ServerInfo {
    pub name: String,
    pub version: String
}