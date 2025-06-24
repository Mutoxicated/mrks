#ifndef HELPERS
#define HELPERS

#include "macros.h"

typedef struct{
    char* array;
    int length;
} StrBuf;

StrBuf* NewStrBuf();
void Write(StrBuf* buf, char c);
void WriteString(StrBuf* buf, ...);
void ResetStrBuf(StrBuf* buf);
void FreeStrBuf(StrBuf* buf);
char* GetStrBufString(StrBuf* buf);

typedef struct{
    int min;
    int max;
} Range;
Range NewRange(int min, int max);

ARRAY_DEF(Range, Ranges)
ARRAY_DEF(int, ints)

typedef struct{
    char* str;
    int length;
    ints* strStarts;
    int strLen;
} Strings;

Strings* NewStrings(char* str);
void AddString(Strings* strings, char* str);
char* GetString(Strings* strings, int index);
void FreeStrings(Strings* strings);

char* itoa(int num, int base);

#endif