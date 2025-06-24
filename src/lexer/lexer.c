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

    char o = fgetc(file);

    while (EOF == EOF) {
        if (isalpha(o)) {
            AddChar(identBuf, o);
        }else if (strcmp(identBuf->array, "") != 0) {
            AddToken(tokens, NewToken(Identifier, identBuf->array));
            ResetStrBuf(identBuf);
        }
        if (o == EOF) {
            break;
        }
        o = fgetc(file);
    }

    fclose(file);
    FreeStrBuf(identBuf);
    return tokens;
}

