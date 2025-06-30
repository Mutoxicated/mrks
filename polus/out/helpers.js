"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.get_extension = void 0;
function get_extension(uri) {
    var extension = "";
    for (let i = uri.length - 1; i >= 0; i--) {
        if (uri[i] == '.') {
            break;
        }
        extension += uri[i];
    }
    return extension;
}
exports.get_extension = get_extension;
//# sourceMappingURL=helpers.js.map