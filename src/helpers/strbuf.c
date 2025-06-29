#include <stdlib.h>
#include <stdarg.h>
#include "strbuf.h"
#include "string.h"
#include "dbg_options.h"

StrBuf strbuf_new() {
    StrBuf buf;
    buf.array = (char*)malloc(sizeof(char));
    buf.array[0] = '\0';
    buf.length = 0;
    buf.cap = 1;
    return buf;
}

StrBuf strbuf_new_cap(int cap) {
    StrBuf buf;
    buf.array = (char*)malloc(sizeof(char)*(cap+1));
    buf.array[0] = '\0';
    buf.length = 0;
    buf.cap = cap;
    return buf;
}

void strbuf_write(StrBuf* buf, char c) {
    buf->length++;
    if (buf->length > buf->cap-1) {
        buf->cap++;
        buf->array = (char*)realloc(buf->array, sizeof(char)*(buf->cap+1));
    }
    buf->array[buf->length-1] = c;
    buf->array[buf->length] = '\0';
}
/* Variadic parameters MUST ALL BE char* and at the end there should be a NULL */
void _strbuf_write_string(StrBuf* buf, ...) {
    va_list args;
    va_start(args, buf);
    char* arg;
    int strLen = 0;
    while ( (arg = va_arg(args, char*)) != 0) {
        if (arg == NULL) {
            break;
        }
        strLen += strlen(arg);
    }
    va_end(args);
    
    int startIndex = buf->length;
    buf->length += strLen;
    if (buf->length > buf->cap-1) {
        buf->cap = buf->length+1;
        buf->array = (char*)realloc(buf->array, sizeof(char)*(buf->cap+1));
    }

    va_start(args, buf);
    while ( (arg = va_arg(args, char*)) ) {
        if (arg == NULL) {
            break;
        }
        int strLenArg = strlen(arg);
        for (int i = 0; i < strLenArg; i++) {
            buf->array[startIndex+i] = arg[i]; 
        }
        startIndex += strLenArg;
    }
    va_end(args);
   
    buf->array[buf->length] = '\0';
}

void strbuf_reset(StrBuf* buf) {
    buf->array[0] = '\0';
    buf->length = 0;
}

void strbuf_free_contents(StrBuf* buf) {
    free(buf->array);
    buf->array = NULL;
    buf->length = 0;
}