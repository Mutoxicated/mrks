#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "lexer.h"
#include "tokens.h"
#include "helpers.h"
#include "color.h"
#include "dbg_options.h"

const char* Tokenize(FILE* file, Tokens* tokens) {    
    int line = 1;
    int columnEnd = 0;
    StrBuf* identBuf = strbuf_new();
    StrBuf* numBuf = strbuf_new();
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
                strbuf_reset(identBuf);
                continue;
            }
            if (o == '\"') {
                isString = false;
                tokens_add(tokens, token_new(Str, identBuf->array, columnEnd-1, line));
                strbuf_reset(identBuf);
                continue;
            }
            strbuf_write(identBuf, o);
            continue;
        }
        
        if (isalpha(o)) {
            strbuf_write(identBuf, o);
            continue;
        }
        if (strcmp(identBuf->array, "") != 0) {
            Token token = token_new(Identifier, identBuf->array, columnEnd, line);
            TokenType tt = is_ident_keyword(token.lexeme);
            if (tt != Invalid) {
                token.type = tt;
            }
            tokens_add(tokens, token);
            strbuf_reset(identBuf);
        }
        if (isdigit(o)) {
            strbuf_write(numBuf, o);
            continue;
        }
        if (strcmp(numBuf->array, "") != 0) {
            tokens_add(tokens, token_new(Number, numBuf->array, columnEnd, line));
            strbuf_reset(numBuf);
        }
        if (o == EOF) {
            break;
        }
        switch (o) {
            case '\"':
                isString = true;
                break;
            case '=':
                tokens_add(tokens, token_new(Equal, "=", columnEnd, line));
                break;
            case ',':
                tokens_add(tokens, token_new(Comma, ",", columnEnd, line));
                break;
            case '(':
                tokens_add(tokens, token_new(OpenParen, "(", columnEnd, line));
                break;
            case ')':
                tokens_add(tokens, token_new(OpenParen, ")", columnEnd, line));
                break;
            case ':':
                tokens_add(tokens, token_new(Colon, ":", columnEnd, line));
                break;
            case ';':
                tokens_add(tokens, token_new(Semicolon, ";", columnEnd, line));
                break;
            case '-':
                tokens_add(tokens, token_new(Slash, "-", columnEnd, line));
                break;
            case '>':
                tokens_add(tokens, token_new(Greater, ">", columnEnd, line));
                break;
            case '<':
                tokens_add(tokens, token_new(Less, "<", columnEnd, line));
                break;
            case '^':
                tokens_add(tokens, token_new(Caret, "^", columnEnd, line));
                break;
            case '*':
                tokens_add(tokens, token_new(Asterisk, "*", columnEnd, line));
                break;
            case '$':
                tokens_add(tokens, token_new(Dollar, "$", columnEnd, line));
                break;
            case '\n':
                line++;
                columnEnd = 0;
                break;
        }
    }
    tokens_add(tokens, token_new(Eof, "", columnEnd, line));

    fclose(file);
    strbuf_free(identBuf);
    strbuf_free(numBuf);
    return NULL;
}

