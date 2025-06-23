#include "tokens.h"
#include <stdio.h>
#include <stdlib.h>

Token* NewToken(TokenType type, char* lexeme) {
    Token* token = malloc(sizeof(Token));
    token->type = type;
    //(*token).lexeme = strcopy(lexeme);
    return token;
}

Tokens* NewTokens() {
    Tokens* tokens = malloc(sizeof(Tokens));
    Token* innerArray = malloc(sizeof(Token));
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
    free(tokens->array);
    tokens->array = NULL;
    tokens->length = 0;
    tokens = NULL;
}
