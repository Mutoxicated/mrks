#include "nodes.h"
#include "parser.h"
#include <stdbool.h>

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

    Stmts* stmts = NewStmts();

    Token t;
    while (true) {
        t = tget(&ts);
        if (t.type == Eof) {
            break;
        }
    }

    VariableDecl vd;
    vd.core = NewSimpleNodeCore(t);
    Stmt stmt;
    stmt.vd = vd;
    stmt.type = VariableDeclaration;

    AddStmt(stmts, stmt);

    return stmts;
}
