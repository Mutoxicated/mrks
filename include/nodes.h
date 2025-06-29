#ifndef NODES
#define NODES 

#include "macros.h"
#include "helpers.h"
#include "tokens.h"
#include "strbuf.h"

#define FOREACH_EXPR(NODE) \
    NODE(IdentifierExpression) \
    NODE(LiteralExpression) \

#define FOREACH_STMT(NODE) \
    NODE(VariableDeclaration) \

typedef enum {
    FOREACH_EXPR(GENERATE_ENUM)
} ExprType;

static const char* exprStrings[] = {
    FOREACH_EXPR(GENERATE_STRING)
};

typedef enum {
    FOREACH_STMT(GENERATE_ENUM)
} StmtType;

static const char* stmtStrings[] = {
    FOREACH_STMT(GENERATE_STRING)
};

typedef struct{
    Range columnRange;
    Range lineRange;
} NodeLocation;

void node_location_to_str(StrBuf* buf, NodeLocation* nl);

typedef struct{
    Token token;
    TokenLocations locations;
} NodeCore;

NodeCore nodecore_new(Tokens* tokens);
NodeCore nodecore_simple_new(Token token);
void nodecore_free_contents(NodeCore* nc);

/// @brief Gives the full location of the `core`, however many lines that spans
NodeLocation nodecore_get_full_location(NodeCore* core);
/// @brief Gives only the first line location of the `core`
NodeLocation nodecore_get_line_location(NodeCore* core);

typedef struct{
    NodeCore core;
} NodeIdentifier;
NodeIdentifier node_identifier_new(Token t);
void node_identifier_free_contents(NodeIdentifier* ni);

typedef struct{
    NodeCore core;
} NodeLiteral;
NodeLiteral node_literal_new(Token t);

typedef struct Expr Expr;
struct Expr {
    union{
        NodeIdentifier identifier;
        NodeLiteral literal;
    } inner;
    ExprType type;
};

void expr_free_contents(ExprType type, void* any);

// FLAG 1 D 191
typedef struct { 
    Expr* array; 
    int length; 
} Exprs; 
Exprs* exprs_new(); 
void exprs_add(Exprs* arr, Expr token);
void exprs_free(Exprs* arr);
void exprs_free_contents(Exprs* arr);
// END: DON'T MANIPULATE THIS AREA!

// FLAG 2 D 305
typedef struct { 
    NodeIdentifier* array; 
    int length; 
} NodeIdentifiers; 
NodeIdentifiers* node_identifiers_new(); 
void node_identifiers_add(NodeIdentifiers* arr, NodeIdentifier token);
void node_identifiers_free(NodeIdentifiers* arr);
void node_identifiers_free_contents(NodeIdentifiers* arr);
// END: DON'T MANIPULATE THIS AREA!

typedef struct VariableDecl VariableDecl;
struct VariableDecl {
    NodeCore core;
    NodeIdentifiers identifiers;
    Exprs expressions;
};

VariableDecl variable_decl_new(Token t);
void variable_decl_free_contents(VariableDecl* vd);

typedef struct Stmt Stmt;
struct Stmt{
    union{
        VariableDecl vd;
    } inner;
    StmtType type;
};

void stmt_free_contents(StmtType type, void* any);

void expr_to_string(StrBuf* buf, Expr expr);
void stmt_to_string(StrBuf* buf, Stmt stmt);

/* The `any` parameter should be a pointer to a node statement */
Stmt node_into_stmt(StmtType type, void* any);
/* The `any` parameter should be a pointer to a node expression */
Expr node_into_expr(ExprType type, void* any);

// FLAG 3 D 191
typedef struct { 
    Stmt* array; 
    int length; 
} Stmts; 
Stmts* stmts_new(); 
void stmts_add(Stmts* arr, Stmt token);
void stmts_free(Stmts* arr);
void stmts_free_contents(Stmts* arr);
// END: DON'T MANIPULATE THIS AREA!

#endif