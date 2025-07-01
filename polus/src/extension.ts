import * as path from "path";
import { workspace, ExtensionContext } from "vscode";
import * as vscode from "vscode"
import {
  LanguageClient,
  LanguageClientOptions,
  StreamInfo,
} from "vscode-languageclient/node";
import * as net from "net"

// Connect to the LSP
var lsp = new net.Socket();
lsp.connect(30000, '127.0.0.1', function () {
  console.log('Connected');
});
var lspReceivedData = "";
function onData(data: Buffer) {
  console.log('Received: ' + data);
  lspReceivedData = data.toString();
}

let textdocs: vscode.TextDocument[]
let docSelector = { language: '*' }
let client: LanguageClient;
export function activate(context: ExtensionContext) {
	let serverOptions = () => {
		var socket = lsp.connect(30000, '127.0.0.1', function () {
			console.log('Connected');
		});
		let result:StreamInfo = {
			writer: socket,
			reader: socket,
		}
		return Promise.resolve(result)
	}

	const clientOptions: LanguageClientOptions = {
		documentSelector: [docSelector],
		synchronize: {
			fileEvents: workspace.createFileSystemWatcher("**/.clientrc"),
		},
	};

	client = new LanguageClient("polus", serverOptions, clientOptions);
}

export function deactivate(): Thenable<void> | undefined {
	if (!client) {
		return undefined;
	}
	return client.stop();
}
