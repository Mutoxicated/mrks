#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "nodes.h"
#include "helpers.h"
#include "macros.h"
#include "dbg_options.h"
#include "strbuf.h"
#include "color.h"

// FLAG 1 D 628
Exprs* exprs_new() { 
    Exprs* arr = malloc(sizeof(Exprs)); 
    Expr* innerArray = NULL; 
    arr->array = innerArray; 
    arr->length = 0; 

    return arr; 
} 

void exprs_add(Exprs* arr, Expr token) { 
    arr->length++; 
    if (arr->array == NULL) { 
        arr->array = malloc(sizeof(Expr)); 
        arr->array[0] = token;
 
        return; 
    } 
    arr->array = realloc(arr->array, sizeof(Expr)*arr->length); 
    arr->array[arr->length-1] = token; 
} 

void exprs_free(Exprs* arr) {
    if (arr->array != NULL) {
        free(arr->array);
        arr->array = NULL;
    }
    arr->length = 0;
    arr = NULL;
}
// END: DON'T MANIPULATE THIS AREA!

// FLAG 2 D 751
NodeIdentifiers* node_identifiers_new() { 
    NodeIdentifiers* arr = malloc(sizeof(NodeIdentifiers)); 
    NodeIdentifier* innerArray = NULL; 
    arr->array = innerArray; 
    arr->length = 0; 

    return arr; 
} 

void node_identifiers_add(NodeIdentifiers* arr, NodeIdentifier token) { 
    arr->length++; 
    if (arr->array == NULL) { 
        arr->array = malloc(sizeof(NodeIdentifier)); 
        arr->array[0] = token;
 
        return; 
    } 
    arr->array = realloc(arr->array, sizeof(NodeIdentifier)*arr->length); 
    arr->array[arr->length-1] = token; 
} 

void node_identifiers_free(NodeIdentifiers* arr) {
    if (arr->array != NULL) {
        free(arr->array);
        arr->array = NULL;
    }
    arr->length = 0;
    arr = NULL;
}
// END: DON'T MANIPULATE THIS AREA!

// FLAG 3 D 628
Stmts* stmts_new() { 
    Stmts* arr = malloc(sizeof(Stmts)); 
    Stmt* innerArray = NULL; 
    arr->array = innerArray; 
    arr->length = 0; 

    return arr; 
} 

void stmts_add(Stmts* arr, Stmt token) { 
    arr->length++; 
    if (arr->array == NULL) { 
        arr->array = malloc(sizeof(Stmt)); 
        arr->array[0] = token;
 
        return; 
    } 
    arr->array = realloc(arr->array, sizeof(Stmt)*arr->length); 
    arr->array[arr->length-1] = token; 
} 

void stmts_free(Stmts* arr) {
    if (arr->array != NULL) {
        free(arr->array);
        arr->array = NULL;
    }
    arr->length = 0;
    arr = NULL;
}
// END: DON'T MANIPULATE THIS AREA!

Stmt node_into_stmt(StmtType type, void* any) {
    Stmt stmt = {.type = type};
    
    switch (type) {
        case VariableDeclaration:
            stmt.inner.vd = *(VariableDecl*)(any);
    }
    return stmt;
}


char* node_location_to_str(NodeLocation* nl) {
    StrBuf _buf = strbuf_new();
    StrBuf* buf = &_buf;
    strbuf_write_string(buf, itoa(nl->columnRange.min, 10), ":", itoa(nl->lineRange.min, 10));

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
            strbuf_write_string(buf, 
                CYAN(), stmtStrings[(int)stmt.type], WHITE(), " ("YELLOW(), vd.core.token.lexeme, WHITE()")",
                " at ", node_location_to_str(&nl), ":\n\t"CYAN("Expressions: \n")WHITE()
            );
            for (int i = 0; i < vd.expressions.length; i++) {
                strbuf_write_string(buf, "\t\t", expr_to_string(vd.expressions.array[i]));
            }
            strbuf_write_string(buf, CYAN()"\tIdentifiers: \n"WHITE());
            for (int i = 0; i < vd.identifiers.length; i++) {
                strbuf_write_string(buf, "\t\t", vd.identifiers.array[i].core.token.lexeme);
            }
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

/* NODE CONSTRUCTORS */
VariableDecl variable_decl_new(Token t) {
    VariableDecl vd;
    vd.core = nodecore_simple_new(t);
    vd.expressions.array = NULL;
    vd.expressions.length = 0;
    vd.identifiers.array = NULL;
    vd.identifiers.length = 0;
    return vd;
}