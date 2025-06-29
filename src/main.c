#include <stdio.h>
#include "tokens.h"
#include "lexer.h"
#include "compiler.h"
#include "color.h"
#include "dbg_options.h"
 
int main(int argc, char const *argv[]) {
    const char* err = compiler_action("test.k");
    if (err != NULL) {
        printf(RED("Compilation error")WHITE("%s"), err);
    }
    #if MEM_DBG_ENABLED == true
    #include "dbg_internal.h"
    dbg_write_meminfo();
    dbg_free_meminfo();
    #endif
    return err != NULL ? -1 : 0;
}
