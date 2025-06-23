#include <stdlib.h>
#include "helpers.h"

StrBuf* NewStrBuf() {
    StrBuf* buf = malloc(sizeof(StrBuf));
    buf->array = NULL;
    buf->length = 0;
    return buf;
}

void AddChar(StrBuf* buf, char c) {
    if (buf->length == 0) {
        buf->array = malloc(sizeof(char));
        buf->length = 1;
        buf->array[0] = c;
        return;
    }
    buf->length++;
    buf->array = realloc(buf->array, sizeof(char)*buf->length);
    buf->array[buf->length-1] = c;
}
