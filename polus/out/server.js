"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const node_1 = require("vscode-languageserver/node");
require("vscode-uri");
const vscode_languageserver_textdocument_1 = require("vscode-languageserver-textdocument");
const helpers_1 = require("./helpers");
const semantic_token_info_1 = require("./semantic_token_info");
const net = require("net");
// Connect to the LSP
var client = new net.Socket();
client.connect(30000, '127.0.0.1', function () {
    console.log('Connected');
});
var lspReceivedData = "";
function onData(data) {
    console.log('Received: ' + data);
    lspReceivedData = data.toString();
}
const sleep = (ms) => new Promise((resolve) => {
    setTimeout(resolve, ms);
});
const connection = (0, node_1.createConnection)(node_1.ProposedFeatures.all);
connection.onInitialize((_) => {
    const result = {
        capabilities: {
            textDocumentSync: node_1.TextDocumentSyncKind.Full,
            semanticTokensProvider: { legend: { tokenTypes: semantic_token_info_1.tokenTypesLegend, tokenModifiers: semantic_token_info_1.tokenModifiersLegend }, full: true, range: false },
        },
    };
    return result;
});
let documents = new node_1.TextDocuments(vscode_languageserver_textdocument_1.TextDocument);
documents.onDidChangeContent((changeEvent) => {
    console.log(changeEvent.document.uri);
    if ((0, helpers_1.get_extension)(changeEvent.document.uri) != "k") {
        return;
    }
    var text = changeEvent.document.getText();
    client = new net.Socket();
    client.connect(30000, '127.0.0.1', async function () {
        console.log('Connected');
        client.on('data', onData);
        client.write(text);
    });
    sleep(150);
    (0, semantic_token_info_1.SetDocSemantic)(changeEvent.document.uri, lspReceivedData);
});
connection.onRequest("textDocument/semanticTokens/full", (params) => {
    console.log(params.textDocument.uri);
    const tokensBuilder = new node_1.SemanticTokensBuilder();
    var txtdoc = params.textDocument;
    var received = (0, semantic_token_info_1.GetDocSemantic)(txtdoc.uri);
    if (received == "") {
        return tokensBuilder.build();
    }
    var tempBuf = "";
    var tokenColumnStart = -1;
    var tokenColumnEnd = -1;
    var tokenLine = -1;
    var tokenType = "";
    for (let i = 0; i < received.length; i++) {
        var test = received[i];
        if (test == '-') {
            tokenColumnStart = parseInt(tempBuf);
            tempBuf = "";
            continue;
        }
        if (received[i].toString() == ':') {
            tokenColumnEnd = parseInt(tempBuf);
            tempBuf = "";
            continue;
        }
        if (received[i].toString() == '|') {
            tokenLine = parseInt(tempBuf);
            tempBuf = "";
            continue;
        }
        if (received[i].toString() == '\n') {
            tokenType = tempBuf;
            tempBuf = "";
        }
        if (tokenType == "") {
            tempBuf = tempBuf.concat(received[i]);
            continue;
        }
        if (tokenType == "Identifier") {
            tokensBuilder.push(tokenLine, tokenColumnStart, tokenColumnEnd - tokenColumnStart + 1, 0, 0);
        }
        else if (tokenType == "Rubric") {
            tokensBuilder.push(tokenLine, tokenColumnStart, tokenColumnEnd - tokenColumnStart + 1, 1, 0);
        }
        tokenType = "";
    }
    tempBuf = "";
    return tokensBuilder.build();
});
documents.listen(connection);
connection.listen();
//# sourceMappingURL=server.js.map