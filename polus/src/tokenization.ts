import * as vscode from 'vscode';
import { get_extension } from './helpers';
import * as net from "net";

var client = new net.Socket();
client.connect(30000, '127.0.0.1', function() {
	console.log('Connected');
	client.write('Hello, server! Love, Client.');
	client.destroy()
});

function onData(data: Buffer) {
	console.log('Received: ' + data);
}

const tokenTypes = ['class', 'interface', 'enum', 'function', 'variable', 'keyword'];
const tokenModifiers = ['declaration', 'documentation'];
const legend = new vscode.SemanticTokensLegend(tokenTypes, tokenModifiers);

const provider: vscode.DocumentSemanticTokensProvider = {
	provideDocumentSemanticTokens(
		document: vscode.TextDocument
	): vscode.ProviderResult<vscode.SemanticTokens> {
		const tokensBuilder = new vscode.SemanticTokensBuilder(legend);
		if (get_extension(document.uri.toString()) != "k") {
			return tokensBuilder.build();
		}
		var text = document.getText();
        client = new net.Socket()
        client.connect(30000, '127.0.0.1', function() {
            console.log('Connected');
		    client.write(text);
			client.on('data', onData);
        });

		return tokensBuilder.build();
	}
};

const selector = { language: '*', scheme: 'file' };

vscode.languages.registerDocumentSemanticTokensProvider(selector, provider, legend);