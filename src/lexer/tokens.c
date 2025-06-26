#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "tokens.h"
#include "dbg_options.h"

// FLAG 2 D 739
TokenLocations* token_locations_new() { 
    TokenLocations* arr = malloc(sizeof(TokenLocations)); 
    TokenLocation* innerArray = NULL; 
    arr->array = innerArray; 
    arr->length = 0; 

    return arr; 
} 

void token_locations_add(TokenLocations* arr, TokenLocation token) { 
    arr->length++; 
    if (arr->array == NULL) { 
        arr->array = malloc(sizeof(TokenLocation)); 
        arr->array[0] = token;
 
        return; 
    } 
    arr->array = realloc(arr->array, sizeof(TokenLocation)*arr->length); 
    arr->array[arr->length-1] = token; 
} 

void token_locations_free(TokenLocations* arr) {
    if (arr->array != NULL) {
        free(arr->array);
        arr->array = NULL;
    }
    arr->length = 0;
    arr = NULL;
}
// END: DON'T MANIPULATE THIS AREA!

// FLAG 1 D 640
Tokens* tokens_new() { 
    Tokens* arr = malloc(sizeof(Tokens)); 
    Token* innerArray = NULL; 
    arr->array = innerArray; 
    arr->length = 0; 

    return arr; 
} 

void tokens_add(Tokens* arr, Token token) { 
    arr->length++; 
    if (arr->array == NULL) { 
        arr->array = malloc(sizeof(Token)); 
        arr->array[0] = token;
 
        return; 
    } 
    arr->array = realloc(arr->array, sizeof(Token)*arr->length); 
    arr->array[arr->length-1] = token; 
} 

void tokens_free(Tokens* arr) {
    if (arr->array != NULL) {
        free(arr->array);
        arr->array = NULL;
    }
    arr->length = 0;
    arr = NULL;
}
// END: DON'T MANIPULATE THIS AREA!

TokenLocation token_location_new(Range columnRange, int line) {
    TokenLocation tl;
    tl.columnRange = columnRange;
    tl.line = line;
    return tl;
}

Token token_new(TokenType type, char* lexeme, int columnEnd, int line) {
    int len = strlen(lexeme);

    Token token;
    token.type = type;

    token.lexeme = malloc(sizeof(char)*len);
    strcpy(token.lexeme, lexeme);

    token.location = token_location_new(range_new(columnEnd-len, columnEnd), line);
    return token;
}

TokenType is_ident_keyword(char* ident) {
    bool isLower = islower(ident[0]);
    TokenType res = Invalid;
    if (isLower) 
        ident[0] = toupper(ident[0]);
        
    for (int i = 0; i < TOKENS_LENGTH; i++) {
        if (i >= (int)Identifier) {
            goto exit;
        }
        if (strcmp(ident, TOKENS_STRINGS[i]) == 0) {
            res = (TokenType)i;
            goto exit;
        }
    }
    exit:
    if (isLower)
        ident[0] = tolower(ident[0]);
    return res;
}