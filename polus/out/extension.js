"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.deactivate = exports.activate = void 0;
const vscode_1 = require("vscode");
const node_1 = require("vscode-languageclient/node");
const net = require("net");
// Connect to the LSP
var lsp = new net.Socket();
let textdocs;
let docSelector = { language: '*' };
let client;
function activate(context) {
    let serverOptions = () => {
        var socket = lsp.connect(30000, '127.0.0.1');
        let result = {
            writer: socket,
            reader: socket,
        };
        return Promise.resolve(result);
    };
    const clientOptions = {
        documentSelector: [docSelector],
        synchronize: {
            fileEvents: vscode_1.workspace.createFileSystemWatcher("**/.clientrc"),
        },
    };
    client = new node_1.LanguageClient("polus", serverOptions, clientOptions);
    client.start();
}
exports.activate = activate;
function deactivate() {
    if (!client) {
        return undefined;
    }
    return client.stop();
}
exports.deactivate = deactivate;
//# sourceMappingURL=extension.js.map