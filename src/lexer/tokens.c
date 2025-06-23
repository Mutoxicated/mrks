#include "tokens.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Token NewToken(TokenType type, char* lexeme) {
    Token token;
    token.type = type;
    strcpy(token.lexeme, lexeme);
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
