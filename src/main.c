#include <stdio.h>
#include "tokens.h"
#include "lexer.h"
#include "compiler.h"
 
int main(int argc, char const *argv[]) {
    Strings* filesToBuild = strings_new("test.k");
    strings_add(filesToBuild, "oo.k");
    int exitCode = compiler_action(filesToBuild);
    strings_free(filesToBuild);
    return exitCode;
}
