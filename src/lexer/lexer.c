#include <stdlib.h>
#include <stdio.h>
#include "lexer.h"
#include "tokens.c"

Token* tokenize(const char* fileName) {
    Tokens* tokens = NewTokens();

    FILE* file = fopen(fileName, "r");
    if(file == NULL) {
        printf("Not able to open the file.");
        return tokens;
    }
    printf("Opened file.");

    char o = fgetc(file);

    AddToken(tokens, *NewToken(Eof, ""));

    printf("%s", tokens->array[0].lexeme);
    
    fclose(file);

    return tokens;
}