#include "nodes.h"
#include "parser.h"
#include <stdbool.h>
#include "dbg_options.h"

typedef struct{
    Tokens* tokens;
    int index;
    int cap;
} TokensScanner;

Token tget(TokensScanner* ts) {
    Token t = ts->tokens->array[ts->index];
    ts->index++;
    return t;
}

Stmts* Parse(Tokens* tokens) {
    TokensScanner ts;
    ts.tokens = tokens;
    ts.index = 0;
    ts.cap = sizeof(*tokens)/sizeof(Token);

    Stmts* stmts = stmts_new();

    Token t;
    while (true) {
        t = tget(&ts);
        if (t.type == Eof) {
            break;
        }
    }

    VariableDecl vd;
    vd.core = nodecore_simple_new(t);
    Stmt stmt;
    stmt.inner.vd = vd;
    stmt.type = VariableDeclaration;
    stmts_add(stmts, stmt);

    return stmts;
}
