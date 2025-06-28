#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "compiler.h"
#include "helpers.h"
#include "tokens.h"
#include "lexer.h"
#include "nodes.h"
#include "parser.h"
#include "color.h"
#include "dbg_options.h"
#include "strbuf.h"

const char* compiler_action(char* filename) {
    const char* err = NULL;

    char cwd[_PC_PATH_MAX];
    getcwd(cwd, sizeof(cwd));

    bool isAtEnd = false;
    printf(BOLD_WHITE("-> File: %s\n") WHITE(), filename);
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return "Couldn't open the file.";
    }
    Tokens* tokens = tokens_new();
    err = Tokenize(file, tokens);
    fclose(file);
    if (err != NULL) {
        goto exit;
    }
    printf(BOLD_WHITE("--------------------------------------> Tokens:\n") WHITE());
    for (int i = 0; i < tokens->length; i++) {
        Token token = tokens->array[i];
        TokenLocation location = token.location;
        printf(CYAN("%s") WHITE(" at %d-%d:%d:") YELLOW(" \"%s\"\n"), TOKENS_STRINGS[(int)token.type], location.columnRange.min, location.columnRange.max, location.line, token.lexeme);
    }
    printf(WHITE());

    Stmts* stmts = Parse(tokens);
    printf(BOLD_WHITE("--------------------------------------> Nodes:\n") WHITE());
    StrBuf buf = strbuf_new();
    for (int i = 0; i < stmts->length; i++) {
        Stmt stmt = stmts->array[i];
        stmt_to_string(&buf, stmt);
        printf("%s\n", buf.array);
        strbuf_reset(&buf);
    }
    strbuf_free_contents(&buf);
    stmts_free(stmts);
    exit:
    tokens_free(tokens);
    return err;
}