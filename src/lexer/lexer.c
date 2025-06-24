#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "lexer.h"
#include "tokens.h"
#include "helpers.h"

const char* Tokenize(const char* fileName, Tokens* tokens) {
    FILE* file = fopen(fileName, "r");
    if(file == NULL) {
        return "Not able to open the file.";
    }
    printf("Opened file.\n");
    
    int line = 1;
    int columnEnd = 0;
    StrBuf* identBuf = NewStrBuf();
    StrBuf* numBuf = NewStrBuf();
    bool isString = false;

    char o;
    while (true) {
        o = fgetc(file);
        columnEnd++;

        if (isString) {
            if (o == '\n') {
                line++;
                columnEnd = 0;
                isString = false;
                ResetStrBuf(identBuf);
                continue;
            }
            if (o == '\"') {
                isString = false;
                AddToken(tokens, NewToken(Str, identBuf->array, columnEnd-1, line));
                ResetStrBuf(identBuf);
                continue;
            }
            Write(identBuf, o);
            continue;
        }
        
        if (isalpha(o)) {
            Write(identBuf, o);
            continue;
        }
        if (strcmp(identBuf->array, "") != 0) {
            Token token = NewToken(Identifier, identBuf->array, columnEnd, line);
            TokenType tt = IdentIsKeyword(token.lexeme);
            if (tt != Invalid) {
                token.type = tt;
            }
            AddToken(tokens, token);
            ResetStrBuf(identBuf);
        }
        if (isdigit(o)) {
            Write(numBuf, o);
            continue;
        }
        if (strcmp(numBuf->array, "") != 0) {
            AddToken(tokens, NewToken(Number, numBuf->array, columnEnd, line));
            ResetStrBuf(numBuf);
        }
        if (o == EOF) {
            break;
        }
        switch (o) {
            case '\"':
                isString = true;
                break;
            case '=':
                AddToken(tokens, NewToken(Equal, "=", columnEnd, line));
                break;
            case ',':
                AddToken(tokens, NewToken(Comma, ",", columnEnd, line));
                break;
            case '(':
                AddToken(tokens, NewToken(OpenParen, "(", columnEnd, line));
                break;
            case ')':
                AddToken(tokens, NewToken(OpenParen, ")", columnEnd, line));
                break;
            case ':':
                AddToken(tokens, NewToken(Colon, ":", columnEnd, line));
                break;
            case ';':
                AddToken(tokens, NewToken(Semicolon, ";", columnEnd, line));
                break;
            case '-':
                AddToken(tokens, NewToken(Slash, "-", columnEnd, line));
                break;
            case '>':
                AddToken(tokens, NewToken(Greater, ">", columnEnd, line));
                break;
            case '<':
                AddToken(tokens, NewToken(Less, "<", columnEnd, line));
                break;
            case '^':
                AddToken(tokens, NewToken(Caret, "^", columnEnd, line));
                break;
            case '*':
                AddToken(tokens, NewToken(Asterisk, "*", columnEnd, line));
                break;
            case '$':
                AddToken(tokens, NewToken(Dollar, "$", columnEnd, line));
                break;
            case '\n':
                line++;
                columnEnd = 0;
                break;
        }
    }
    AddToken(tokens, NewToken(Eof, "", columnEnd, line));

    fclose(file);
    FreeStrBuf(identBuf);
    FreeStrBuf(numBuf);
    return NULL;
}

