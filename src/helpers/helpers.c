#include <stdlib.h>
#include "helpers.h"

StrBuf* NewStrBuf() {
    StrBuf* buf = malloc(sizeof(StrBuf));
    buf->array = malloc(sizeof(char));
    buf->array[0] = '\0';
    buf->length = 0;
    return buf;
}

void AddChar(StrBuf* buf, char c) {
    buf->length++;
    buf->array = realloc(buf->array, sizeof(char)*(buf->length+1));
    buf->array[buf->length-1] = c;
    buf->array[buf->length] = '\0';
}

void ResetStrBuf(StrBuf* buf) {
    buf->array[0] = '\0';
    buf->length = 0;
}

void FreeStrBuf(StrBuf* buf) {
    free(buf->array);
    buf->array = NULL;
    buf->length = 0;
    free(buf);
}