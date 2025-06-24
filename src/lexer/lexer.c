#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "lexer.h"
#include "tokens.h"
#include "helpers.h"

Tokens* tokenize(const char* fileName) {
    Tokens* tokens = NewTokens();

    FILE* file = fopen(fileName, "r");
    if(file == NULL) {
        printf("Not able to open the file.\n");
        return tokens;
    }
    printf("Opened file.\n");

    StrBuf* identBuf = NewStrBuf();
    StrBuf* numBuf = NewStrBuf();

    char o;
    while (EOF == EOF) {
        o = fgetc(file);
        
        if (isalpha(o)) {
            AddChar(identBuf, o);
        }else if (strcmp(identBuf->array, "") != 0) {
            AddToken(tokens, NewToken(Identifier, identBuf->array));
            ResetStrBuf(identBuf);
        }
        if (isdigit(o)) {
            AddChar(numBuf, o);
        }else if (strcmp(numBuf->array, "") != 0) {
            AddToken(tokens, NewToken(Identifier, numBuf->array));
            ResetStrBuf(numBuf);
        }
        if (o == EOF) {
            break;
        }
        switch (o) {
            case '=':
                AddToken(tokens, NewToken(Equal, "="));
                break;
            case ',':
                AddToken(tokens, NewToken(Comma, ","));
                break;
            case '(':
                AddToken(tokens, NewToken(OpenParen, "("));
                break;
            case ')':
                AddToken(tokens, NewToken(OpenParen, ")"));
                break;
        }
    }

    fclose(file);
    FreeStrBuf(identBuf);
    FreeStrBuf(numBuf);
    return tokens;
}

