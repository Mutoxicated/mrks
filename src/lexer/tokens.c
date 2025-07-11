#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "tokens.h"
#include "dbg_options.h"

// FLAG 2 D 953
TokenLocations* token_locations_new() { 
    TokenLocations* arr = (TokenLocations*)malloc(sizeof(TokenLocations)); 
    TokenLocation* innerArray = NULL; 
    arr->array = innerArray; 
    arr->length = 0; 

    return arr; 
} 

void token_locations_add(TokenLocations* arr, TokenLocation token) { 
    arr->length++; 
    if (arr->array == NULL) { 
        arr->array = (TokenLocation*)malloc(sizeof(TokenLocation)); 
        arr->array[0] = token;
 
        return; 
    } 
    arr->array = (TokenLocation*)realloc(arr->array, sizeof(TokenLocation)*arr->length); 
    arr->array[arr->length-1] = token; 
} 

void token_locations_free(TokenLocations* arr) {
    if (arr->array != NULL) {
        free(arr->array);
        arr->array = NULL;
    }
    free(arr);
    arr = NULL;
}

void token_locations_free_contents(TokenLocations* arr) {
    if (arr->array != NULL) {
        free(arr->array);
        arr->array = NULL;
    }
    arr->length = 0;
}
// END: DON'T MANIPULATE THIS AREA!

Tokens* tokens_new() { 
    Tokens* arr = (Tokens*)malloc(sizeof(Tokens)); 
    Token* innerArray = NULL; 
    arr->array = innerArray; 
    arr->length = 0; 

    return arr; 
} 

void tokens_add(Tokens* arr, Token token) { 
    arr->length++; 
    if (arr->array == NULL) { 
        arr->array = (Token*)malloc(sizeof(Token)); 
        arr->array[0] = token;
        return; 
    } 
    arr->array = (Token*)realloc(arr->array, sizeof(Token)*arr->length); 
    arr->array[arr->length-1] = token; 
} 

void tokens_free(Tokens* arr) {
    if (arr->array != NULL) {
        for (int i = 0; i < arr->length; i++) {
            free(arr->array[i].lexeme);
        }
        free(arr->array);
        arr->array = NULL;
    }
    free(arr);
    arr = NULL;
}

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

    token.lexeme = (char*)malloc(sizeof(char)*len);
    strcpy(token.lexeme, lexeme);

    token.location = token_location_new(strcmp(lexeme, "(newline)") == 0 || strcmp(lexeme, "(tab)") == 0 ? range_new(columnEnd-1, columnEnd) : range_new(columnEnd-len, columnEnd), line);
    return token;
}

void token_free_contents(Token* t) {
    free(t->lexeme);
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