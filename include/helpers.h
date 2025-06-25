#ifndef HELPERS
#define HELPERS

#include "macros.h"

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

ARRAY_DEF(Range, Ranges, ranges)
ARRAY_DEF(int, ints, ints)

typedef struct{
    char* str;
    int length;
    ints* strStarts;
    int strLen;
} Strings;
Strings* strings_new(char* str);
void strings_add(Strings* strings, char* str);
char* strings_get_by_index(Strings* strings, int index);
void strings_free(Strings* strings);

char* itoa(int num, int base);

#endif