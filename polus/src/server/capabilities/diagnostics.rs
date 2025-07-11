use serde::{Serialize};

use crate::server::client::Range;


#[macro_export] macro_rules! DOC_DIAGNOSTIC_KIND_FULL { () => { String::from("full") }; }
#[macro_export] macro_rules! DOC_DIAGNOSTIC_KIND_UNCHANGED { () => { String::from("unchanged") }; }

#[macro_export] macro_rules! DOC_DIAGNOSTIC_SEVERITY { 
    (ERROR) => { 1 }; 
    (WARNING) => { 2 }; 
    (INFORMATION) => { 3 }; 
    (HINT) => { 4 }; 
}

#[derive(Serialize)]
pub struct Diagnostic {
    pub range: Range,
    pub severity: u8,
    pub code: Option<String>,
    pub message: String,
}

impl Diagnostic {
    pub fn warning(range: Range, message: String) -> Self {
        Self {
            range,
            message,
            severity: DOC_DIAGNOSTIC_SEVERITY!(WARNING),
            code: None
        }
    }
}

#[derive(Serialize)]
pub struct FullDocumentDiagnosticReport {
    pub kind: String,
    #[serde(rename = "resultId")]
    pub result_id: Option<String>,
    pub items: Vec<Diagnostic>,
}

#[derive(Serialize)]
pub struct UnchangedDocumentDiagnosticReport {
    pub kind: String,
    #[serde(rename = "resultId")]
    pub result_id: String
}

#[derive(Serialize)]
pub struct RelatedUnchangedDocumentDiagnosticReport {
    // #[serde(rename = "relatedDocuments")]
    // pub related_documents: idk,
}