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
    Stmts* stmts;
    StrBuf buf;

    char* file_contents;
    fseek(file, 0L, SEEK_END);
    int length = ftell(file);
    fseek(file, 0L, SEEK_SET);
    file_contents = (char*)malloc(length*sizeof (char));
    fread(file_contents, 1, length, file);
    fclose(file);
    Tokenize(file_contents, tokens);
    free(file_contents);
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

    stmts = Parse(tokens);
    printf(BOLD_WHITE("--------------------------------------> Nodes:\n") WHITE());
    buf = strbuf_new();
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