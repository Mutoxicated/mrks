use enum_util::magic;
use lazy_static::lazy_static;
use std::vec::Vec;

#[magic(TOKEN_TYPES)]
pub enum TokenTypeLegend {
    Type,
    Class,
    Enum,
    Interface,
    Struct,
    TypeParameter,
    Parameter,
    Variable,
    Property,
    EnumMember,
    Event,
    Function,
    Method,
    Keyword,
    Modifier,
    Comment,
    String,
    Number,
    Regexp,
    Operator,
    Decorator,
}
/*
...
impl Into < String > for TokenTypeLegend {
    fn into (self) -> String {
    match self {
        TokenTypeLegend :: Decorator => "decorator" . to_owned () , 
        TokenTypeLegend :: Operator => "operator" . to_owned () , 
        TokenTypeLegend :: Regexp => "regexp" . to_owned () , 
        TokenTypeLegend :: Number => "number" . to_owned () , 
        TokenTypeLegend :: String => "string" . to_owned () , 
        TokenTypeLegend :: Comment => "comment" . to_owned () , 
        TokenTypeLegend :: Modifier => "modifier" . to_owned () , 
        TokenTypeLegend :: Keyword => "keyword" . to_owned () , T
        okenTypeLegend :: Method => "method" . to_owned () , 
        TokenTypeLegend :: Function => "function" . to_owned () , 
        TokenTypeLegend :: Event => "event" . to_owned () , 
        TokenTypeLegend :: EnumMember => "enummember" . to_owned () , 
        TokenTypeLegend :: Property => "property" . to_owned () , 
        TokenTypeLegend :: Variable => "variable" . to_owned () , 
        TokenTypeLegend :: Parameter => "parameter" . to_owned () , T
        okenTypeLegend :: TypeParameter => "typeparameter" . to_owned () , 
        TokenTypeLegend :: Struct => "struct" . to_owned () , 
        TokenTypeLegend :: Interface => "interface" . to_owned () , 
        TokenTypeLegend :: Enum => "enum" . to_owned () , 
        TokenTypeLegend :: Class => "class" . to_owned () , 
        TokenTypeLegend :: Type => "type" . to_owned ()}}} 
        
        lazy_static ! {
            pub static ref raCompletionMarkerTOKEN_TYPES : Vec < String > = vec ! ["decorator" . to_owned () , "operator" . to_owned () , "regexp" . to_owned () , "number" . to_owned () , "string" . to_owned () , "comment" . to_owned () , "modifier" . to_owned () , "keyword" . to_owned () , "method" . to_owned () , "function" . to_owned () , "event" . to_owned () , "enummember" . to_owned () , "property" . to_owned () , "variable" . to_owned () , "parameter" . to_owned () , "typeparameter" . to_owned () , "struct" . to_owned () , "interface" . to_owned () , "enum" . to_owned () , "class" . to_owned () , "type" . to_owned ()] ;
        }
    }
}
*/

#[magic(TOKEN_MODIFIERS)]
pub enum TokenModifierLegend {
    Declaration,
    Definition,
    Readonly,
    Static,
    Deprecated,
    Abstract,
    Async,
    Modification,
    Documentation,
    DefaultLibrary
}

