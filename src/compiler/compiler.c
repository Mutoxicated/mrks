#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include "compiler.h"
#include "helpers.h"
#include "tokens.h"
#include "lexer.h"
#include "nodes.h"
#include "parser.h"

int Compile(Strings* filesToBuild) {
    char cwd[_PC_PATH_MAX];
    getcwd(cwd, sizeof(cwd));

    bool isAtEnd = false;
    for (int i = 0; i < filesToBuild->length; i++) {
        char* file = GetString(filesToBuild, i);
        printf("File: %s\n", file);
        
        Tokens* tokens = NewTokens();
        const char* err = Tokenize(file, tokens);
        if (err != NULL) {
            printf("Compilation error: %s\n", err);
            return -1;
        }
        for (int i = 0; i < tokens->length; i++) {
            Token token = tokens->array[i];
            TokenLocation location = token.location;
            printf("%s at %d-%d:%d: \"%s\"\n", TokenStrings[(int)token.type], location.columnRange.min, location.columnRange.max, location.line, token.lexeme);
        }

        Stmts* stmts = Parse(tokens);
        for (int i = 0; i < stmts->length; i++) {
            Stmt stmt = stmts->array[i];
            printf("%s\n", Stmt2String(stmt));
        }

        FreeTokens(tokens);
    }

    return 0;
}