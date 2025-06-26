#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "nodes.h"
#include "helpers.h"
#include "macros.h"
#include "dbg_options.h"

ARRAY_IMPL(Expr, Exprs, exprs)
ARRAY_IMPL(Stmt, Stmts, stmts)
ARRAY_IMPL(NodeIdentifier, NodeIdentifiers, node_identifiers)

char* node_location_to_str(NodeLocation* nl) {
    StrBuf _buf = strbuf_new();
    StrBuf* buf = &_buf;
    strbuf_write_string(buf, itoa(nl->columnRange.min, 10), ":", itoa(nl->lineRange.min, 10), NULL);

    char* str = strbuf_get_str(buf);
    strbuf_free(buf);
    return str;
}

NodeLocation nodecore_get_line_location(NodeCore* nc) {
    NodeLocation nl;
    nl.columnRange = nc->locations.array[0].columnRange;
    nl.lineRange = range_new(nc->locations.array[0].line, nc->locations.array[0].line);

    for (int i = 1; i < nc->locations.length; i++) {
        if (nc->locations.array[0].line != nl.lineRange.min) {
            break;
        }
        Range colRange = nc->locations.array[i].columnRange;
        if (nl.columnRange.max < colRange.max) {
            nl.columnRange.max = colRange.max;
        }
    }

    return nl;
}

char* expr_to_string(Expr expr) {
    return "";
}

char* stmt_to_string(Stmt stmt) {
    StrBuf _buf = strbuf_new();
    StrBuf* buf = &_buf;
    switch (stmt.type) {
        case VariableDeclaration:
            VariableDecl vd = stmt.inner.vd;
            NodeLocation nl = nodecore_get_line_location(&vd.core);
            strbuf_write_string(buf, stmtStrings[(int)stmt.type], " at ", node_location_to_str(&nl), ":", NULL);
            break;
    }
    char* res = strbuf_get_str(buf);
    strbuf_free(buf);
    return res;
}

NodeCore nodecore_new(Tokens* tokens) {
    NodeCore nc;
    nc.token = tokens->array[0];
    nc.locations.array = NULL;
    nc.locations.length = 0;
    for (int i = 0; i < tokens->length; i++) {
        token_locations_add(&nc.locations, tokens->array[i].location);
    }

    return nc;
}
NodeCore nodecore_simple_new(Token token) {
    NodeCore nc;
    nc.token = token;
    nc.locations.array = NULL;
    nc.locations.length = 0;
    token_locations_add(&nc.locations, token.location);

    return nc;
}