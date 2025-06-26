#ifndef NODES
#define NODES 

#include "macros.h"
#include "helpers.h"
#include "tokens.h"

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

char* node_location_to_str(NodeLocation* nl);

typedef struct{
    Token token;
    TokenLocations locations;
} NodeCore;

NodeCore nodecore_new(Tokens* tokens);
NodeCore nodecore_simple_new(Token token);

/// @brief Gives the full location of the `core`, however many lines that spans
NodeLocation nodecore_get_full_location(NodeCore* core);
/// @brief Gives only the first line location of the `core`
NodeLocation nodecore_get_line_location(NodeCore* core);

typedef struct{
    NodeCore core;
} NodeIdentifier;

typedef struct{
    NodeCore core;
} NodeLiteral;

typedef struct Expr Expr;

struct Expr {
    union{
        NodeIdentifier identifier;
        NodeLiteral literal;
    } inner;
    ExprType type;
};

// FLAG 1 D 153
typedef struct { 
    Expr* array; 
    int length; 
} Exprs; 
Exprs* exprs_new(); 
void exprs_add(Exprs* arr, Expr token);
void exprs_free(Exprs* arr);
// END: DON'T MANIPULATE THIS AREA!

// FLAG 2 D 246
typedef struct { 
    NodeIdentifier* array; 
    int length; 
} NodeIdentifiers; 
NodeIdentifiers* node_identifiers_new(); 
void node_identifiers_add(NodeIdentifiers* arr, NodeIdentifier token);
void node_identifiers_free(NodeIdentifiers* arr);
// END: DON'T MANIPULATE THIS AREA!

typedef struct VariableDecl VariableDecl;
struct VariableDecl {
    NodeCore core;
    NodeIdentifiers identifiers;
    Exprs expressions;
};

VariableDecl variable_decl_new(Token t);

typedef struct Stmt Stmt;

struct Stmt{
    union{
        VariableDecl vd;
    } inner;
    StmtType type;
};

VariableDecl VarDecl(Stmt stmt);

char* expr_to_string(Expr expr);
char* stmt_to_string(Stmt stmt);

Stmt node_into_stmt(StmtType type, void* any);

// FLAG 3 D 153
typedef struct { 
    Stmt* array; 
    int length; 
} Stmts; 
Stmts* stmts_new(); 
void stmts_add(Stmts* arr, Stmt token);
void stmts_free(Stmts* arr);
// END: DON'T MANIPULATE THIS AREA!

#endif