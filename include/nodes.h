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

char* NodeLocToString(NodeLocation* nl);

typedef struct{
    Token token;
    TokenLocations* locations;
} NodeCore;

NodeCore NewNodeCore(Tokens* tokens);
NodeCore NewSimpleNodeCore(Token token);

/// @brief Gives the full location of the `core`, however many lines that spans
NodeLocation GetFullLocation(NodeCore core);
/// @brief Gives only the first line location of the `core`
NodeLocation GetLineLocation(NodeCore core);

typedef struct{
    NodeCore core;
} NodeIdentifier;
ARRAY_DEF(NodeIdentifier, NodeIdentifiers)

typedef struct{
    NodeCore core;
} NodeLiteral;

typedef union{
    NodeIdentifier identifier;
    NodeLiteral literal;
    ExprType type;
} Expr;
ARRAY_DEF(Expr, Exprs)

typedef struct{
    NodeCore core;
    NodeIdentifiers identifiers;
    Exprs expressions;
} VariableDecl;

typedef union{
    VariableDecl vd;
    StmtType type;
} Stmt;
ARRAY_DEF(Stmt, Stmts)

char* Expr2String(Expr expr);
char* Stmt2String(Stmt stmt);

#endif