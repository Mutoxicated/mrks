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
        TOKEN(Comma)  \
        TOKEN(Dollar) \
        TOKEN(Colon) \
        TOKEN(Semicolon) \
        \
        TOKEN(Equal)   \
        TOKEN(Less)  \
        TOKEN(Greater)  \
        \
        TOKEN(Plus) \
        TOKEN(Dash)  \
        TOKEN(Slash) \
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

// FLAG 2 D 492
typedef struct { 
    TokenLocation* array; 
    int length; 
} TokenLocations; 
TokenLocations* token_locations_new(); 
void token_locations_add(TokenLocations* arr, TokenLocation token);
void token_locations_free(TokenLocations* arr);
void token_locations_free_contents(TokenLocations* arr);
// END: DON'T MANIPULATE THIS AREA!

typedef struct {
    TokenType type;
    char* lexeme;
    TokenLocation location;
} Token;

void token_free_contents(Token* t);

// FLAG 1 D 202
typedef struct { 
    Token* array; 
    int length; 
} Tokens; 
Tokens* tokens_new(); 
void tokens_add(Tokens* arr, Token token);
void tokens_free(Tokens* arr);
void tokens_free_contents(Tokens* arr);
// END: DON'T MANIPULATE THIS AREA!

Token token_new(TokenType type, char* lexeme, int columnEnd, int line);
TokenType is_ident_keyword(char* ident);

#endif