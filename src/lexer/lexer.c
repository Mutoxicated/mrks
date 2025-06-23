#include <stdlib.h>
#include <stdio.h>
#include "lexer.h"
#include "tokens.h"

Tokens* tokenize(const char* fileName) {
    Tokens* tokens = NewTokens();

    FILE* file = fopen(fileName, "r");
    if(file == NULL) {
        printf("Not able to open the file.\n");
        return tokens;
    }
    printf("Opened file.\n");

    char o = fgetc(file);

    printf("%s", tokens->array[0].lexeme);
    
    fclose(file);

    return tokens;
}