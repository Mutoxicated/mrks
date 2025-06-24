#include "tokens.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

Token NewToken(TokenType type, char* lexeme, int columnEnd, int line) {
    int len = strlen(lexeme);

    Token token;
    token.type = type;

    token.lexeme = malloc(sizeof(char)*len);
    strcpy(token.lexeme, lexeme);

    token.line = line;
    token.columnRange = NewRange(columnEnd-len, columnEnd);
    return token;
}

Tokens* NewTokens() {
    Tokens* tokens = malloc(sizeof(Tokens));
    Token* innerArray = NULL;
    tokens->array = innerArray;
    tokens->length = 0;

    return tokens;
}

void AddToken(Tokens* tokens, Token token) {
    tokens->length++;
    if (tokens->array == NULL) {
        tokens->array = malloc(sizeof(Token));
        tokens->array[0] = token;
        return;
    }
    tokens->array = realloc(tokens->array, sizeof(Token)*tokens->length);
    tokens->array[tokens->length-1] = token;
}

void FreeTokens(Tokens* tokens) {
    if (tokens->array != NULL) {
        free(tokens->array);
        tokens->array = NULL;
    }
    tokens->length = 0;
    tokens = NULL;
}

TokenType IdentIsKeyword(char* ident) {
    bool isLower = islower(ident[0]);
    TokenType res = Invalid;
    if (isLower) 
        ident[0] = toupper(ident[0]);
        
    for (int i = 0; i < TokensLen; i++) {
        if (i >= (int)Identifier) {
            goto exit;
        }
        if (strcmp(ident, TokenStrings[i]) == 0) {
            res = (TokenType)i;
            goto exit;
        }
    }
    exit:
    if (isLower)
        ident[0] = tolower(ident[0]);
    return res;
}