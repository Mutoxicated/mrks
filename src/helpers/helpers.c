#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include "helpers.h"
#include "dbg_options.h"

Range range_new(int min, int max) {
    Range range;
    range.min = min;
    range.max = max;
    return range;
}

StrBuf strbuf_new() {
    StrBuf buf;
    buf.array = malloc(sizeof(char));
    buf.array[0] = '\0';
    buf.length = 0;
    buf.cap = 1;
    return buf;
}

StrBuf strbuf_new_cap(int cap) {
    StrBuf buf;
    buf.array = malloc(sizeof(char)*(cap+1));
    buf.array[0] = '\0';
    buf.length = 0;
    buf.cap = cap;
    return buf;
}

void strbuf_write(StrBuf* buf, char c) {
    buf->length++;
    if (buf->length > buf->cap-1) {
        buf->cap++;
        buf->array = realloc(buf->array, sizeof(char)*(buf->cap+1));
    }
    buf->array[buf->length-1] = c;
    buf->array[buf->length] = '\0';
}
/* Variadic parameters MUST ALL BE char* and at the end there should be a NULL */
void strbuf_write_string(StrBuf* buf, ...) {
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
        buf->array = realloc(buf->array, sizeof(char)*(buf->cap+1));
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

void strbuf_free(StrBuf* buf) {
    free(buf->array);
    buf->array = NULL;
    buf->length = 0;
}

char* strbuf_get_str(StrBuf* buf) {
    char* str = malloc(sizeof(char)*buf->length);
    strcpy(str, buf->array);
    return str;
}

ARRAY_IMPL(Range, Ranges, ranges)
ARRAY_IMPL(int, ints, ints)

Strings* strings_new(char* str) {
    Strings* strings = malloc(sizeof(Strings));
    int len = strlen(str);
    strings->str = malloc(sizeof(char)*len);
    strcpy(strings->str, str);
    strings->length = 1;
    strings->strStarts = ints_new();
    ints_add(strings->strStarts, 0);
    strings->strLen = strlen(str);

    return strings;
}

void strings_add(Strings* strings, char* str) {
    strings->strLen += sizeof(str);
    strings->str = realloc(strings->str, strings->strLen+1);
    
    int currentStrIndex = 0;
    int index = 0;
    for (int i = 0; i < strings->strLen; i++) {
        if (currentStrIndex == strings->length) {
            if (i == strings->strLen-1) {
                strings->str[i] = '\0';
                break;
            }
            strings->str[i] = str[index];
            index++;
            continue;
        }
        
        if (strings->str[i] == '\0') {
            currentStrIndex++;
            if (currentStrIndex == strings->length) {
                ints_add(strings->strStarts, i+1);
            }
        }
    }

    strings->length++;
}

char* strings_get_by_index(Strings* strings, int index) {
    StrBuf buf = strbuf_new();
    StrBuf* bufptr = &buf;

    int i = strings->strStarts->array[index];
    while (true) {
        if (strings->str[i] == '\0') {
            break;
        }
        strbuf_write(bufptr, strings->str[i]);
        i++;
    }

    char* string = malloc(sizeof(*(buf.array)));
    strcpy(string, buf.array);

    strbuf_free(bufptr);
    return string;
}

void strings_free(Strings* strings) {
    free(strings->str);
    ints_free(strings->strStarts);
    free(strings);
    strings = NULL;
}

void reverse(char str[], int length) {
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        end--;
        start++;
    }
}

char* itoa(int num, int base) {
    StrBuf buf = strbuf_new();
    StrBuf* bufptr = &buf;

    bool isNegative = false;

    if (num == 0) {
        strbuf_write(bufptr, '0');
        goto exit;
    }

    // In standard itoa(), negative numbers are handled
    // only with base 10. Otherwise numbers are
    // considered unsigned.
    if (num < 0 && base == 10) {
        isNegative = true;
        num = -num;
    }

    while (num != 0) {
        int rem = num % base;
        strbuf_write(bufptr, rem > 9 ? (rem - 10) + 'a' : rem + '0');
        num = num / base;
    }

    if (isNegative)
        strbuf_write(bufptr, '-');

    exit:
    char* str = strbuf_get_str(bufptr);
    reverse(str, buf.length);

    strbuf_free(bufptr);
    return str;
}
