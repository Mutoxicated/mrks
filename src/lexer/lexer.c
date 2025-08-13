#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "lexer.h"
#include "helpers.h"
#include "color.h"
#include "dbg_options.h"
#include "strbuf.h"
#include "tokens.h"

void Tokenize(char* src, Tokens* tokens) { 
    int line = 1;
    int columnEnd = 0;
    StrBuf _ident_buf = strbuf_new();
    StrBuf* ident_buf = &_ident_buf;
    StrBuf _num_buf = strbuf_new();
    StrBuf* num_buf = &_num_buf;
    bool is_string = false;
    bool is_comment = false;
    bool is_multiline_comment = false;

    char o;
    int index = -1;
    int len = strlen(src)-1;
    while (true) {
        index++;
        columnEnd++;
        if (index > len) {
            if (strcmp(_ident_buf.array, "") != 0) {
                Token token = token_new(Identifier, _ident_buf.array, columnEnd-1, line);
                TokenType tt = is_ident_keyword(token.lexeme);
                if (tt != Invalid) {
                    token.type = tt;
                }
                tokens_add(tokens, token);
                strbuf_reset(ident_buf);
            }else if (strcmp(_num_buf.array, "") != 0) {
                tokens_add(tokens, token_new(Number, _num_buf.array, columnEnd-1, line));
                strbuf_reset(num_buf);
            }
            break;
        }
        o = src[index];

        if (is_multiline_comment) {
            if (o == '\n') {
                line++;
                columnEnd = 0;
            }
            if (o == '*' && index != len && src[index+1]  == '/') {
                index++;
                is_multiline_comment = false;
            }
            continue;
        }
        if (is_comment) {
            if (o == '\n') {
                is_comment = false;
                line++;
                columnEnd = 0;
            }
            if (o == '/' && index != len && src[index+1] == '/') {
                index++;
                is_comment = false;
            }
            continue;
        }
        if (is_string) {
            if (o == '\n') {
                line++;
                columnEnd = 0;
                is_string = false;
                strbuf_reset(ident_buf);
                continue;
            }
            if (o == '\"') {
                is_string = false;
                tokens_add(tokens, token_new(Str, _ident_buf.array, columnEnd-2, line));
                strbuf_reset(ident_buf);
                continue;
            }
            strbuf_write(ident_buf, o);
            continue;
        }
        
        if (isalpha(o)) {
            strbuf_write(ident_buf, o);
            continue;
        }
        if (strcmp(_ident_buf.array, "") != 0) {
            Token token = token_new(Identifier, _ident_buf.array, columnEnd-1, line);
            TokenType tt = is_ident_keyword(token.lexeme);
            if (tt != Invalid) {
                token.type = tt;
            }
            tokens_add(tokens, token);
            strbuf_reset(ident_buf);
        }
        if (isdigit(o)) {
            strbuf_write(num_buf, o);
            continue;
        }
        if (strcmp(_num_buf.array, "") != 0) {
            tokens_add(tokens, token_new(Number, _num_buf.array, columnEnd-1, line));
            strbuf_reset(num_buf);
        }
        switch (o) {
            case '\"':
                is_string = true;
                break;
            case '=':
                tokens_add(tokens, token_new(Equal, "=", columnEnd-1, line));
                break;
            case ',':
                tokens_add(tokens, token_new(Comma, ",", columnEnd-1, line));
                break;
            case '(':
                tokens_add(tokens, token_new(OpenParen, "(", columnEnd-1, line));
                break;
            case ')':
                tokens_add(tokens, token_new(OpenParen, ")", columnEnd-1, line));
                break;
            case ':':
                tokens_add(tokens, token_new(Colon, ":", columnEnd-1, line));
                break;
            case '-':
                tokens_add(tokens, token_new(Dash, "-", columnEnd-1, line));
                break;
            case '>':
                tokens_add(tokens, token_new(Greater, ">", columnEnd-1, line));
                break;
            case '<':
                tokens_add(tokens, token_new(Less, "<", columnEnd-1, line));
                break;
            case '^':
                tokens_add(tokens, token_new(Caret, "^", columnEnd-1, line));
                break;
            case '*':
                tokens_add(tokens, token_new(Asterisk, "*", columnEnd-1, line));
                break;
            case '+':
                tokens_add(tokens, token_new(Dash, "+", columnEnd-1, line));
                break;
            case '/':
                if (index != len) {
                    char next_o = src[index+1];
                    if (next_o == '/') {
                        is_comment = true;
                        break;
                    }else if (next_o == '*') {
                        is_multiline_comment = true;
                        break;
                    }
                    index++;
                }
                tokens_add(tokens, token_new(Slash, "/", columnEnd-1, line));
                break;
            case '$':
                tokens_add(tokens, token_new(Dollar, "$", columnEnd-1, line));
                break;
            case '\n':
                tokens_add(tokens, token_new(Newline, "(newline)", columnEnd-1, line));
                line++;
                columnEnd = 0;
                break;
            case '\t':
                tokens_add(tokens, token_new(Identation, "(tab)", columnEnd-1, line));
                break;

        }
    }
    tokens_add(tokens, token_new(Eof, "", columnEnd, line));

    strbuf_free_contents(ident_buf);
    strbuf_free_contents(num_buf);
}