use lazy_static::lazy_static;
use std::vec::Vec;

pub enum TokenTypeLegend {
    Variable,
    Keyword
}

impl Into<String> for TokenTypeLegend {
    fn into(self) -> String {
        match self {
            TokenTypeLegend::Variable => String::from("variable"),
            TokenTypeLegend::Keyword => String::from("keyword")
        }
    }
}

pub enum TokenModifierLegend {
    Class
}

impl Into<String> for TokenModifierLegend {
    fn into(self) -> String {
        match self {
            TokenModifierLegend::Class => String::from("class")
        }
    }
}

lazy_static!{
    pub static ref TOKEN_TYPES:Vec<String> = vec![TokenTypeLegend::Variable.into(), TokenTypeLegend::Keyword.into()];
    pub static ref TOKEN_MODIFIERS:Vec<String> = vec![TokenModifierLegend::Class.into()];
}