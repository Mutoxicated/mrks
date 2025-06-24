#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "nodes.h"
#include "helpers.h"
#include "macros.h"

ARRAY_IMPL(Expr, Exprs)
ARRAY_IMPL(Stmt, Stmts)
ARRAY_IMPL(NodeIdentifier, NodeIdentifiers)



char* NodeLocToString(NodeLocation* nl) {
    StrBuf* buf = NewStrBuf();
    WriteString(buf, itoa(nl->columnRange.min, 10), ":", itoa(nl->lineRange.min, 10), NULL);

    char* str = GetStrBufString(buf);
    FreeStrBuf(buf);
    return str;
}

NodeLocation GetLineLocation(NodeCore nc) {
    NodeLocation nl;
    nl.columnRange = nc.locations->array[0].columnRange;
    nl.lineRange = NewRange(nc.locations->array[0].line, nc.locations->array[0].line);

    for (int i = 1; i < nc.locations->length; i++) {
        if (nc.locations->array[0].line != nl.lineRange.min) {
            break;
        }
        Range colRange = nc.locations->array[i].columnRange;
        if (nl.columnRange.max < colRange.max) {
            nl.columnRange.max = colRange.max;
        }
    }

    return nl;
}

char* Expr2String(Expr expr) {
    return "";
}

char* Stmt2String(Stmt stmt) {
    StrBuf* buf = NewStrBuf();
    switch (stmt.type) {
        case VariableDeclaration:
            NodeLocation nl = GetLineLocation(stmt.vd.core);
            WriteString(buf, stmtStrings[(int)stmt.type], " at ", NodeLocToString(&nl), NULL);
            break;
    }
    char* res = GetStrBufString(buf);
    FreeStrBuf(buf);
    return res;
}

NodeCore NewNodeCore(Tokens* tokens) {
    NodeCore nc;
    nc.token = tokens->array[0];
    nc.locations = NewTokenLocations();
    for (int i = 0; i < tokens->length; i++) {
        AddTokenLocation(nc.locations, tokens->array[i].location);
    }

    return nc;
}
NodeCore NewSimpleNodeCore(Token token) {
    NodeCore nc;
    nc.token = token;
    nc.locations = NewTokenLocations();
    AddTokenLocation(nc.locations, token.location);

    return nc;
}