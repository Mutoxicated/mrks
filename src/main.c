#include <stdio.h>
#include "tokens.h"
#include "lexer.h"
 
int main(int argc, char const *argv[]) {
    Tokens* tokens = tokenize("test.k");
    for (int i = 0; i < tokens->length; i++) {
        Token token = tokens->array[i];
        printf("%s at %d-%d:%d: \"%s\"\n", TokenStrings[(int)token.type], token.columnRange.min, token.columnRange.max, token.line, token.lexeme);
    }
    FreeTokens(tokens);
    return 0;
}
