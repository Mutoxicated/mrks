#ifndef HELPERS
#define HELPERS

#include "macros.h"

typedef struct { 
    int length; 
    struct{}* array; 
} Array; 

Array array_new(); 
void array_add(Array* arr, struct{} obj); 
struct{}* array_get(Array* arr, int index);
void array_free(Array* arr);

typedef struct{
    char* array;
    int length;
} StrBuf;

StrBuf* strbuf_new();
void strbuf_write(StrBuf* buf, char c);
void strbuf_write_string(StrBuf* buf, ...);
void strbuf_reset(StrBuf* buf);
void strbuf_free(StrBuf* buf);
char* strbuf_get_str(StrBuf* buf);

typedef struct{
    int min;
    int max;
} Range;
Range range_new(int min, int max);

typedef struct{
    char* str;
    int length;
    int strLen;
    Array strStarts;
} Strings;
Strings* strings_new(char* str);
void strings_add(Strings* strings, char* str);
char* strings_get_by_index(Strings* strings, int index);
void strings_free(Strings* strings);

char* itoa(int num, int base);

#endif