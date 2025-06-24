#ifndef MACROS 
#define MACROS

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

#include <stdlib.h>

#define ARRAY_IMPL(TYPE, NAME) \
NAME* New##NAME() { \
    NAME* arr = malloc(sizeof(NAME)); \
    TYPE* innerArray = NULL; \
    arr->array = innerArray; \
    arr->length = 0; \
\
    return arr; \
} \
\
void Add##TYPE(NAME* arr, TYPE token) { \
    arr->length++; \
    if (arr->array == NULL) { \
        arr->array = malloc(sizeof(TYPE)); \
        arr->array[0] = token; \
        return; \
    } \
    arr->array = realloc(arr->array, sizeof(TYPE)*arr->length); \
    arr->array[arr->length-1] = token; \
} \
\
void Free##NAME(NAME* arr) {\
    if (arr->array != NULL) {\
        free(arr->array);\
        arr->array = NULL;\
    }\
    arr->length = 0;\
    arr = NULL;\
}\

#define ARRAY_DEF(TYPE, NAME) typedef struct { TYPE* array; int length; } NAME; NAME* New##NAME(); void Add##TYPE(NAME* arr, TYPE token); void Free##NAME(NAME* arr); \

#endif