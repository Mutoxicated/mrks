"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.GetDocSemantic = exports.SetDocSemantic = exports.tokenModifiersLegend = exports.tokenTypesLegend = void 0;
exports.tokenTypesLegend = [
    'variable', 'keyword'
];
exports.tokenModifiersLegend = [
    'declaration'
];
var documentSemanticTokensMap = new Map();
function SetDocSemantic(uri, info) {
    documentSemanticTokensMap.set(uri, info);
}
exports.SetDocSemantic = SetDocSemantic;
function GetDocSemantic(uri) {
    if (!documentSemanticTokensMap.has(uri)) {
        return "";
    }
    return documentSemanticTokensMap.get(uri);
}
exports.GetDocSemantic = GetDocSemantic;
//# sourceMappingURL=semantic_token_info.js.map