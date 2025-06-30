import * as path from "path";
import { workspace, ExtensionContext } from "vscode";
import * as vscode from 'vscode'

import {
  LanguageClient,
  LanguageClientOptions,
  ServerOptions,
  TransportKind,
} from "vscode-languageclient/node";

let client: LanguageClient;

export function activate(context: ExtensionContext) {
	const serverModule = context.asAbsolutePath(
		path.join("out", "server.js")
	);

	const serverOptions: ServerOptions = {
		run: { module: serverModule, transport: TransportKind.ipc },
		debug: {
		module: serverModule,
		transport: TransportKind.ipc,
		},
	};

	const clientOptions: LanguageClientOptions = {
		documentSelector: [{ language: '*', scheme: 'file'}],
		synchronize: {
		fileEvents: workspace.createFileSystemWatcher("**/.clientrc"),
		},
	};

	client = new LanguageClient(
		"markus",
		"polus",
		serverOptions,
		clientOptions
	);

	client.start();
}

export function deactivate(): Thenable<void> | undefined {
	if (!client) {
		return undefined;
	}
	return client.stop();
}

import "./tokenization"