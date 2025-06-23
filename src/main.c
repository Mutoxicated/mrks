#include <stdio.h>
#include <stdlib.h>
#include "tokens.h"
#include "lexer.h"
 
int main(int argc, char const *argv[]) {
    Tokens* tokens = tokenize("./thing.crazy");
    printf("Hi!\n");
    FreeTokens(tokens);
    return 0;
}
