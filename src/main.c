#include <stdio.h>
#include "tokens.h"
#include "lexer.h"
#include "compiler.h"
 
int main(int argc, char const *argv[]) {
    Strings* filesToBuild = NewStrings("test.k");
    AddString(filesToBuild, "oo.k");
    int exitCode = Compile(filesToBuild);
    FreeStrings(filesToBuild);
    return exitCode;
}
