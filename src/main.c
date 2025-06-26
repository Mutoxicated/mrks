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
    printf(BOLD_WHITE()BLUE("MEMORY ALLOCATIONS:\n")WHITE());
    for (int i = 0; i < mem_array_length; i++) {
        printf("->%s\n", mem_array[i].file);
        printf("\tline: %d\n", mem_array[i].line);
        printf("\tfreed: %s\n", mem_array[i].freed == 0 ? "false" : "true");
    }
    dbg_free_meminfo();
    #endif
    return err != NULL ? -1 : 0;
}
