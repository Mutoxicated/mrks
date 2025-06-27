#ifndef DBG_INTERNAL
#define DBG_INTERNAL
#include <stddef.h>
#include <stdbool.h>

void* dbg_malloc(size_t size, char* file, int line);
void* dbg_realloc(void* ptr, size_t size, char* file, int line);
void dbg_free(void* ptr, char* file, int line);
void dbg_free_meminfo();
void dbg_write_meminfo();

typedef struct{
    char* file;
    int line;
    bool freed;
} DbgMemInfo;

extern void** pointer_array;
extern int pointer_array_length;
extern DbgMemInfo* mem_array;
extern int mem_array_length;

#endif