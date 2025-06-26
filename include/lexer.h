#ifndef LEXER
#define LEXER

#include "tokens.h"
#include <stdio.h>

const char* Tokenize(FILE* file, Tokens* token);

#endif