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

static const char* TOKENS_STRINGS[] = {
    KEYWORDS(GENERATE_STRING)
};

static const int TOKENS_LENGTH = sizeof(TOKENS_STRINGS)/sizeof(TOKENS_STRINGS[0]);

typedef struct {
    Range columnRange;
    int line;
} TokenLocation;
TokenLocation token_location_new(Range columnRange, int line);

typedef struct {
    TokenType type;
    char* lexeme;
    TokenLocation location;
} Token;

// FLAG 1

Token token_new(TokenType type, char* lexeme, int columnEnd, int line);
TokenType is_ident_keyword(char* ident);

#endif