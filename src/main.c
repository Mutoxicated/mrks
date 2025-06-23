#include <stdio.h>
#include <stdlib.h>
#include "tokens.h"
#include "lexer.h"
 
int main(int argc, char const *argv[]) {
    Tokens* tokens = tokenize("thing.crazy");
    for (int i = 0; i < tokens->length; i++) {
        printf("%d %s\n", i, tokens->array[i].lexeme);
    }
    FreeTokens(tokens);
    return 0;
}
