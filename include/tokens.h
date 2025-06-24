#ifndef TOKENS
#define TOKENS

typedef enum TokenType {
    Identifier,
    Number,

    Equal,
    Dot,
    Comma,
    OpenParen,
    CloseParen,

    Eof
} TokenType;

typedef struct Token {
    TokenType type;
    char* lexeme;
} Token;

typedef struct Tokens {
    Token* array;
    int length;
} Tokens;

Tokens* NewTokens();
void AddToken(Tokens* tokens, Token token);
void FreeTokens(Tokens* tokens);

Token NewToken(TokenType type, char* lexeme);

#endif