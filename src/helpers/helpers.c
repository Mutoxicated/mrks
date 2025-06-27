#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "helpers.h"
#include "dbg_options.h"
#include "strbuf.h"

Range range_new(int min, int max) {
    Range range;
    range.min = min;
    range.max = max;
    return range;
}

// FLAG 1 D 760
ints* ints_new() { 
    ints* arr = malloc(sizeof(ints)); 
    int* innerArray = NULL; 
    arr->array = innerArray; 
    arr->length = 0; 

    return arr; 
} 

void ints_add(ints* arr, int token) { 
    arr->length++; 
    if (arr->array == NULL) { 
        arr->array = malloc(sizeof(int)); 
        arr->array[0] = token;
 
        return; 
    } 
    arr->array = realloc(arr->array, sizeof(int)*arr->length); 
    arr->array[arr->length-1] = token; 
} 

void ints_free(ints* arr) {
    if (arr->array != NULL) {
        free(arr->array);
        arr->array = NULL;
    }
    free(arr);
    arr = NULL;
}

void ints_free_contents(ints* arr) {
    if (arr->array != NULL) {
        free(arr->array);
        arr->array = NULL;
    }
    arr->length = 0;
}
// END: DON'T MANIPULATE THIS AREA!

Strings* strings_new(char* str) {
    Strings* strings = malloc(sizeof(Strings));
    int len = strlen(str);
    strings->str = malloc(sizeof(char)*len);
    strcpy(strings->str, str);
    strings->length = 1;
    strings->strStarts.array = NULL;
    strings->strStarts.length = 0;
    ints_add(&strings->strStarts, 0);
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
                ints_add(&strings->strStarts, i+1);
            }
        }
    }

    strings->length++;
}

char* strings_get_by_index(Strings* strings, int index) {
    StrBuf buf = strbuf_new();
    StrBuf* bufptr = &buf;

    int i = strings->strStarts.array[index];
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
    ints_free_contents(&strings->strStarts);
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
