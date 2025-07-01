// import * as vscode from 'vscode';
// import { get_extension } from './helpers';
// import * as net from "net";
// import { SetDocSemantic, tokenModifiersLegend, tokenTypesLegend } from "./semantic_token_info"
// // tokenization code:
// var client = new net.Socket();
// client.connect(30000, '127.0.0.1', function () {
// 	console.log('Connected');
// 	client.write('Hello, server! Love, Client.');
// 	client.destroy()
// });
// var received = "";
// function onData(data: Buffer) {
// 	console.log('Received: ' + data);
// 	received = data.toString();
// }
// const legend = new vscode.SemanticTokensLegend(tokenModifiersLegend, tokenTypesLegend);
// const sleep = (ms: number) => new Promise((resolve) => {
// 	setTimeout(resolve, ms);
// })
// export class Provider implements vscode.DocumentSemanticTokensProvider {
//   provideDocumentSemanticTokens(
// 	document: vscode.TextDocument): vscode.SemanticTokens {
// 	console.log(document.uri);
// 	const tokensBuilder = new vscode.SemanticTokensBuilder();
// 	if (get_extension(document.uri.toString()) != "k") {
// 		return tokensBuilder.build();
// 	}
// 	var text = document.getText();
// 	client = new net.Socket()
// 	client.connect(30000, '127.0.0.1', async function () {
// 		console.log('Connected');
// 		client.on('data', onData);
// 		client.write(text);
// 	});
// 	sleep(100);
// 	SetDocSemantic(document.uri.toString(), received);
// 	return tokensBuilder.build();
//   }
// }
// export const selector = { language: '*' };
//# sourceMappingURL=tokenization.js.map