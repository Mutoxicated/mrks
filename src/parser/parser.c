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

    Token t = tget(&ts);
    VariableDecl vd = variable_decl_new(t);
    while (true) {
        t = tget(&ts);
        if (t.type == Identifier && vd.identifiers.length == 0) {
            node_identifiers_add(&vd.identifiers, node_identifier_new(t));
        }
        if (t.type == Str && vd.expressions.length == 0) {
            NodeLiteral nl = node_literal_new(t);
            exprs_add(&vd.expressions, node_into_expr(LiteralExpression, &nl));
        }
        if (t.type == Eof) {
            break;
        }
    }

    stmts_add(stmts, node_into_stmt(VariableDeclaration, &vd));

    return stmts;
}
