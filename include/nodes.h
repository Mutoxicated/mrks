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
    TokenLocations* locations;
} NodeCore;

NodeCore nodecore_new(Tokens* tokens);
NodeCore nodecore_simple_new(Token token);

/// @brief Gives the full location of the `core`, however many lines that spans
NodeLocation nodecore_get_full_location(NodeCore core);
/// @brief Gives only the first line location of the `core`
NodeLocation nodecore_get_line_location(NodeCore core);

typedef struct{
    NodeCore core;
} NodeIdentifier;
ARRAY_DEF(NodeIdentifier, NodeIdentifiers, node_identifiers)

typedef struct{
    NodeCore core;
} NodeLiteral;

typedef union{
    NodeIdentifier identifier;
    NodeLiteral literal;
    ExprType type;
} ExprInner;

typedef struct{
    ExprType type;
    ExprInner inner;
} Expr;
ARRAY_DEF(Expr, Exprs, exprs)

typedef struct{
    NodeCore core;
    NodeIdentifiers identifiers;
    Exprs expressions;
} VariableDecl;

typedef union{
    VariableDecl vd;
} StmtInner;

typedef struct{
    StmtType type;
    StmtInner inner;
} Stmt;
ARRAY_DEF(Stmt, Stmts, stmts)

char* expr_to_string(Expr expr);
char* stmt_to_string(Stmt stmt);

#endif