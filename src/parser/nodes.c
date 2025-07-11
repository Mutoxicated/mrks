#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "nodes.h"
#include "helpers.h"
#include "macros.h"
#include "dbg_options.h"
#include "strbuf.h"
#include "color.h"

Exprs* exprs_new() { 
    Exprs* arr = (Exprs*)malloc(sizeof(Exprs)); 
    Expr* innerArray = NULL; 
    arr->array = innerArray; 
    arr->length = 0; 

    return arr; 
} 

void exprs_add(Exprs* arr, Expr token) { 
    arr->length++; 
    if (arr->array == NULL) { 
        arr->array = (Expr*)malloc(sizeof(Expr)); 
        arr->array[0] = token;
 
        return; 
    } 
    arr->array = (Expr*)realloc(arr->array, sizeof(Expr)*arr->length); 
    arr->array[arr->length-1] = token; 
} 

void exprs_free(Exprs* arr) {
    if (arr->array != NULL) {
        free(arr->array);
        arr->array = NULL;
    }
    free(arr);
    arr = NULL;
}

void exprs_free_contents(Exprs* arr) {
    if (arr->array != NULL) {
        for (int i = 0; i < arr->length; i++) {
            expr_free_contents(arr->array[i].type, &arr->array[i].inner);
        }
        free(arr->array);
        arr->array = NULL;
    }
    arr->length = 0;
}

NodeIdentifiers* node_identifiers_new() { 
    NodeIdentifiers* arr = (NodeIdentifiers*)malloc(sizeof(NodeIdentifiers)); 
    NodeIdentifier* innerArray = NULL; 
    arr->array = innerArray; 
    arr->length = 0; 

    return arr; 
} 

void node_identifiers_add(NodeIdentifiers* arr, NodeIdentifier token) { 
    arr->length++; 
    if (arr->array == NULL) { 
        arr->array = (NodeIdentifier*)malloc(sizeof(NodeIdentifier)); 
        arr->array[0] = token;
 
        return; 
    } 
    arr->array = (NodeIdentifier*)realloc(arr->array, sizeof(NodeIdentifier)*arr->length); 
    arr->array[arr->length-1] = token; 
} 

void node_identifiers_free(NodeIdentifiers* arr) {
    if (arr->array != NULL) {
        free(arr->array);
        arr->array = NULL;
    }
    free(arr);
    arr = NULL;
}

void node_identifiers_free_contents(NodeIdentifiers* arr) {
    if (arr->array != NULL) {
        for (int i = 0; i < arr->length; i++) {
            node_identifier_free_contents(&arr->array[i]);
        }
        free(arr->array);
        arr->array = NULL;
    }
    arr->length = 0;
}

Stmts* stmts_new() { 
    Stmts* arr = (Stmts*)malloc(sizeof(Stmts)); 
    Stmt* innerArray = NULL; 
    arr->array = innerArray; 
    arr->length = 0; 

    return arr; 
} 

void stmts_add(Stmts* arr, Stmt token) { 
    arr->length++; 
    if (arr->array == NULL) { 
        arr->array = (Stmt*)malloc(sizeof(Stmt)); 
        arr->array[0] = token;
 
        return; 
    } 
    arr->array = (Stmt*)realloc(arr->array, sizeof(Stmt)*arr->length); 
    arr->array[arr->length-1] = token; 
} 

void stmts_free(Stmts* arr) {
    if (arr->array != NULL) {
        for (int i = 0; i < arr->length; i++) {
            stmt_free_contents(arr->array[i].type, &arr->array[i].inner);
        }
        free(arr->array);
        arr->array = NULL;
    }
    free(arr);
    arr = NULL;
}

void expr_free_contents(ExprType type, void* any) {
    switch (type) {
        case IdentifierExpression:
            {
                NodeIdentifier* ni = (NodeIdentifier*)any;
                node_identifier_free_contents(ni);
                break;
            }
        case LiteralExpression:
            NodeLiteral* nl = (NodeLiteral*)any;
            nodecore_free_contents(&nl->core);
            break;
    }
}

void stmt_free_contents(StmtType type, void* any) {
    switch (type){
        case VariableDeclaration:
            VariableDecl* vd = (VariableDecl*)any;
            variable_decl_free_contents(vd);
    }
}

Expr node_into_expr(ExprType type, void* any) {
    Expr expr = {.type = type};

    switch (type) {
        case IdentifierExpression:
            expr.inner.identifier = *(NodeIdentifier*)any;
        case LiteralExpression:
            expr.inner.literal = *(NodeLiteral*)any;
    }
    return expr;
}

Stmt node_into_stmt(StmtType type, void* any) {
    Stmt stmt = {.type = type};
    
    switch (type) {
        case VariableDeclaration:
            VariableDecl* vd = (VariableDecl*)(any);
            stmt.inner.vd = *vd;
    }
    return stmt;
}

void node_location_to_str(StrBuf* buf, NodeLocation* nl) {
    itoa(buf, nl->columnRange.min, 10);
    strbuf_write(buf, ':');
    itoa(buf, nl->lineRange.min, 10);
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

void expr_to_string(StrBuf* buf, Expr expr) {
    switch (expr.type) {
        case IdentifierExpression:
            strbuf_write_string(buf, CYAN()"Identifier: " YELLOW()"\"", expr.inner.identifier.core.token.lexeme, "\"" WHITE());
            break;
        case LiteralExpression:
            strbuf_write_string(buf, CYAN()"Literal: " YELLOW()"\"", expr.inner.literal.core.token.lexeme, "\"" WHITE());
            break;
    }
}

void stmt_to_string(StrBuf* buf, Stmt stmt) {
    StrBuf _inner_buf = strbuf_new();
    StrBuf* inner_buf = &_inner_buf;

    switch (stmt.type) {
        case VariableDeclaration:
            VariableDecl vd = stmt.inner.vd;
            NodeLocation nl = nodecore_get_line_location(&vd.core);

            node_location_to_str(inner_buf, &nl);
            strbuf_write_string(buf, 
                CYAN(), stmtStrings[(int)stmt.type], WHITE(), " (" YELLOW(), vd.core.token.lexeme, WHITE()")",
                " at ", _inner_buf.array, ":\n\t" CYAN("Expressions: \n")WHITE()
            );
            strbuf_reset(inner_buf);

            for (int i = 0; i < vd.expressions.length; i++) {
                expr_to_string(inner_buf, vd.expressions.array[i]);
                strbuf_write_string(buf, "\t\t", _inner_buf.array, "\n");
                strbuf_reset(inner_buf);
            }
            strbuf_write_string(buf, CYAN()"\tIdentifiers: \n" WHITE());
            for (int i = 0; i < vd.identifiers.length; i++) {
                expr_to_string(inner_buf, node_into_expr(IdentifierExpression, &vd.identifiers.array[i]));
                strbuf_write_string(buf, "\t\t", _inner_buf.array, "\n");
                strbuf_reset(inner_buf);
            }
            break;
    }
    strbuf_free_contents(inner_buf);
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
void nodecore_free_contents(NodeCore* nc) {
    token_locations_free_contents(&nc->locations);
}

/* NODE CONSTRUCTORS AND DESTRUCTORS */
VariableDecl variable_decl_new(Token t) {
    VariableDecl vd;
    vd.core = nodecore_simple_new(t);
    vd.expressions.array = NULL;
    vd.expressions.length = 0;
    vd.identifiers.array = NULL;
    vd.identifiers.length = 0;
    return vd;
}

void variable_decl_free_contents(VariableDecl* vd) {
    exprs_free_contents(&vd->expressions);
    node_identifiers_free_contents(&vd->identifiers);
    nodecore_free_contents(&vd->core);
}


NodeIdentifier node_identifier_new(Token t) {
    NodeIdentifier ni;
    ni.core = nodecore_simple_new(t);
    return ni;
}

void node_identifier_free_contents(NodeIdentifier* ni) {
    nodecore_free_contents(&ni->core);
}

NodeLiteral node_literal_new(Token t) {
    NodeLiteral nl;
    nl.core = nodecore_simple_new(t);
    return nl;
}