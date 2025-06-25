#include <stdio.h>
#include "tokens.h"
#include "lexer.h"
#include "compiler.h"
#include "dbg_options.h"
 
int main(int argc, char const *argv[]) {
    Strings* filesToBuild = strings_new("test.k");
    strings_add(filesToBuild, "oo.k");
    int exitCode = compiler_action(filesToBuild);
    strings_free(filesToBuild);
    #if MEM_DBG_ENABLED == true 
    dbg_free_meminfo();
    #endif
    return exitCode;
}
