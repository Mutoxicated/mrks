use proc_macro::TokenStream;
use quote::{format_ident, quote, ToTokens};
use syn::Ident;


#[proc_macro_attribute]
pub fn magic(attr: TokenStream, item: TokenStream) -> TokenStream {
    let mut enm: syn::ItemEnum = syn::parse(item.clone()).expect("Expected enum after magic attribute");

    let name = enm.ident;
    let mut vars:Vec<Ident> = Vec::new();
    let mut lc_vars:Vec<String> = Vec::new();
    for _ in 0..enm.variants.len() {
        let enumvar = enm.variants.pop().unwrap().into_tuple().0.ident;
        let lc_enumvar = enumvar.to_string();
        let mut chars = lc_enumvar.chars().collect::<Vec<char>>();
        let ch = chars[0].to_lowercase().nth(0).unwrap();
        chars[0] = ch;
        let lc_enumvar = String::from_utf16_lossy(chars.iter().map(|x| x.clone() as u16).collect::<Vec<u16>>().as_slice());

        vars.push(enumvar);
        lc_vars.push(lc_enumvar);
    }
    let mut result = TokenStream::from(item);
    let expanded: TokenStream = quote! {
        impl Into<String> for #name {
            fn into(self) -> String {
                match self {
                    #(#name::#vars => #lc_vars.to_owned()),*
                }
            }
        }
    }.into();
    result.extend(expanded.into_iter());
    if !attr.is_empty() {
        let nname:Ident = syn::parse(attr).expect("Expected identifier in magic attributes");
        let expanded: TokenStream = quote! {
            lazy_static! {
                pub static ref #nname:Vec<String> = vec![#(#lc_vars.to_owned()),*];
            }
        }.into();
        result.extend(expanded.into_iter());
    }
    eprintln!("EXPANDED: {}", result.to_string());
    result
}

/*
impl Into<String> for TokenTypeLegend {
    fn into(self) -> String {
        match self {
            TokenTypeLegend::Variable => String::from("variable"),
            TokenTypeLegend::Keyword => String::from("keyword")
        }
    }
}

pub enum TokenModifierLegend {
    Declaration
}

impl Into<String> for TokenModifierLegend {
    fn into(self) -> String {
        match self {
            TokenModifierLegend::Declaration => String::from("declaration")
        }
    }
}

lazy_static!{
    pub static ref TOKEN_TYPES:Vec<String> = vec![TokenTypeLegend::Variable.into(), TokenTypeLegend::Keyword.into()];
    pub static ref TOKEN_MODIFIERS:Vec<String> = vec![TokenModifierLegend::Declaration.into()];
}
*/