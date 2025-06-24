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
        TOKEN(Eof) \
        TOKEN(Invalid) \

#ifndef TOKENS
#define TOKENS

#include "macros.h"
#include "helpers.h"
#include <stdlib.h>

typedef enum {
    KEYWORDS(GENERATE_ENUM)
} TokenType;

static const char* TokenStrings[] = {
    KEYWORDS(GENERATE_STRING)
};

static const int TokensLen = sizeof(TokenStrings)/sizeof(TokenStrings[0]);

typedef struct {
    Range columnRange;
    int line;
} TokenLocation;
TokenLocation NewTokenLocation(Range columnRange, int line);

ARRAY_DEF(TokenLocation, TokenLocations)

typedef struct {
    TokenType type;
    char* lexeme;
    TokenLocation location;
} Token;

ARRAY_DEF(Token, Tokens)

Token NewToken(TokenType type, char* lexeme, int columnEnd, int line);
TokenType IdentIsKeyword(char* ident);

#endif