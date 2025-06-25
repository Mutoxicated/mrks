#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include "compiler.h"
#include "helpers.h"
#include "tokens.h"
#include "lexer.h"
#include "nodes.h"
#include "parser.h"
#include "dbg_options.h"

int compiler_action(Strings* filesToBuild) {
    char cwd[_PC_PATH_MAX];
    getcwd(cwd, sizeof(cwd));

    bool isAtEnd = false;
    for (int i = 0; i < filesToBuild->length; i++) {
        char* file = strings_get_by_index(filesToBuild, i);
        printf("File: %s\n", file);
        
        Tokens* tokens = tokens_new();
        const char* err = Tokenize(file, tokens);
        if (err != NULL) {
            printf("Compilation error: %s\n", err);
            return -1;
        }
        for (int i = 0; i < tokens->length; i++) {
            Token token = tokens->array[i];
            TokenLocation location = token.location;
            printf("%s at %d-%d:%d: \"%s\"\n", TOKENS_STRINGS[(int)token.type], location.columnRange.min, location.columnRange.max, location.line, token.lexeme);
        }

        Stmts* stmts = Parse(tokens);
        for (int i = 0; i < stmts->length; i++) {
            Stmt stmt = stmts->array[i];
            printf("%s\n", stmt_to_string(stmt));
        }

        tokens_free(tokens);
    }

    return 0;
}