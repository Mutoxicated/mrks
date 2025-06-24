#define KEYWORDS(TOKEN) \
        TOKEN(Rubric)  \
        TOKEN(Global)  \
        TOKEN(Let)  \
        TOKEN(Be)  \
        TOKEN(Function) \
        TOKEN(If) \
        \
        TOKEN(Identifier)   \
        TOKEN(Str)   \
        TOKEN(Number)  \
        \
        TOKEN(OpenParen)  \
        TOKEN(CloseParen)  \
        \
        TOKEN(Dot)  \
        TOKEN(Slash)  \
        TOKEN(Comma)  \
        TOKEN(Dollar) \
        TOKEN(Colon) \
        TOKEN(Semicolon) \
        \
        TOKEN(Equal)   \
        TOKEN(Less)  \
        TOKEN(Greater)  \
        TOKEN(Asterisk) \
        TOKEN(Caret) \
        \
        TOKEN(Invalid) \

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

#ifndef TOKENS
#define TOKENS

#include "helpers.h"

typedef enum {
    KEYWORDS(GENERATE_ENUM)
} TokenType;

static char* TokenStrings[] = {
    KEYWORDS(GENERATE_STRING)
};

static const int TokensLen = sizeof(TokenStrings)/sizeof(TokenStrings[0]);

typedef struct {
    TokenType type;
    char* lexeme;
    Range columnRange;
    int line;
} Token;

typedef struct {
    Token* array;
    int length;
} Tokens;

Tokens* NewTokens();
void AddToken(Tokens* tokens, Token token);
void FreeTokens(Tokens* tokens);
Token NewToken(TokenType type, char* lexeme, int columnEnd, int line);

TokenType IdentIsKeyword(char* ident);

#endif