#ifndef MACROS 
#define MACROS

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

#include <stdlib.h>

#define ARRAY_IMPL(TYPE, NAME, LNAME) \
NAME* LNAME##_new() { \
    NAME* arr = malloc(sizeof(NAME)); \
    TYPE* innerArray = NULL; \
    arr->array = innerArray; \
    arr->length = 0; \
\
    return arr; \
} \
\
void LNAME##_add(NAME* arr, TYPE token) { \
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
void LNAME##_free(NAME* arr) {\
    if (arr->array != NULL) {\
        free(arr->array);\
        arr->array = NULL;\
    }\
    arr->length = 0;\
    arr = NULL;\
}\

#define ARRAY_DEF(TYPE, NAME, LNAME) typedef struct { TYPE* array; int length; } NAME; NAME* LNAME##_new(); void LNAME##_add(NAME* arr, TYPE token); void LNAME##_free(NAME* arr); \

#endif