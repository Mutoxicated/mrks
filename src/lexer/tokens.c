#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "tokens.h"

ARRAY_IMPL(Token, Tokens)
ARRAY_IMPL(TokenLocation, TokenLocations)

TokenLocation NewTokenLocation(Range columnRange, int line) {
    TokenLocation tl;
    tl.columnRange = columnRange;
    tl.line = line;
    return tl;
}

Token NewToken(TokenType type, char* lexeme, int columnEnd, int line) {
    int len = strlen(lexeme);

    Token token;
    token.type = type;

    token.lexeme = malloc(sizeof(char)*len);
    strcpy(token.lexeme, lexeme);

    token.location = NewTokenLocation(NewRange(columnEnd-len, columnEnd), line);
    return token;
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