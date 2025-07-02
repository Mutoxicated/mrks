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

let textdocs: vscode.TextDocument[]
let docSelector = { language: '*' }
let client: LanguageClient;
export function activate(context: ExtensionContext) {
	let serverOptions = () => {
		var socket = lsp.connect(30000, '127.0.0.1');
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

	client.start()
}

export function deactivate(): Thenable<void> | undefined {
	if (!client) {
		return undefined;
	}
	return client.stop();
}
