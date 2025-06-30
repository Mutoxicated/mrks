
export function get_extension(uri: string): string {
    var extension = ""

    for (let i = uri.length-1; i >= 0; i--) {
        if (uri[i] == '.') {
        break;
        }
        extension += uri[i];
    }

    return extension
}