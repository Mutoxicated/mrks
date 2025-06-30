"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const vscode = require("vscode");
const helpers_1 = require("./helpers");
const net = require("net");
var client = new net.Socket();
client.connect(30000, '127.0.0.1', function () {
    console.log('Connected');
    client.write('Hello, server! Love, Client.');
    client.destroy();
});
function onData(data) {
    console.log('Received: ' + data);
}
const tokenTypes = ['class', 'interface', 'enum', 'function', 'variable', 'keyword'];
const tokenModifiers = ['declaration', 'documentation'];
const legend = new vscode.SemanticTokensLegend(tokenTypes, tokenModifiers);
const provider = {
    provideDocumentSemanticTokens(document) {
        const tokensBuilder = new vscode.SemanticTokensBuilder(legend);
        if ((0, helpers_1.get_extension)(document.uri.toString()) != "k") {
            return tokensBuilder.build();
        }
        var text = document.getText();
        client = new net.Socket();
        client.connect(30000, '127.0.0.1', function () {
            console.log('Connected');
            client.write(text);
            client.on('data', onData);
        });
        return tokensBuilder.build();
    }
};
const selector = { language: '*', scheme: 'file' };
vscode.languages.registerDocumentSemanticTokensProvider(selector, provider, legend);
//# sourceMappingURL=tokenization.js.map