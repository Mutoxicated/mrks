#ifndef HELPERS
#define HELPERS

#include "macros.h"
#include "strbuf.h"

typedef struct{
    int min;
    int max;
} Range;
Range range_new(int min, int max);

// FLAG 1 D 081
typedef struct { 
    int* array; 
    int length; 
} ints; 
ints* ints_new(); 
void ints_add(ints* arr, int token);
void ints_free(ints* arr);
void ints_free_contents(ints* arr);
// END: DON'T MANIPULATE THIS AREA!

typedef struct{
    char* str;
    int length;
    int strLen;
    ints strStarts;
} Strings;
Strings* strings_new(char* str);
void strings_add(Strings* strings, char* str);
char* strings_get_by_index(Strings* strings, int index);
void strings_free(Strings* strings);

char* itoa(StrBuf* buf, int num, int base);

#endif