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

    while (o != EOF) {
        if (isalpha(o)) {
        }else if (strcmp(identBuf, "") == 0) {
            AddToken(tokens, NewToken(Identifier, identBuf));
            identBuf = NewStrBuf();
        }
        o = fgetc(file);
    }

    printf("%s", tokens->array[0].lexeme);
    
    fclose(file);

    free(identBuf);
    return tokens;
}

