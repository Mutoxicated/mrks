#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "dbg_internal.h"

void** pointer_array = NULL;
int pointer_array_length = 0;
DbgMemInfo* mem_array = NULL;
int mem_array_length = 0;

void* dbg_malloc(size_t size, char* file, int line) {
    void* ptr = malloc(size+sizeof(int));
    pointer_array_length++;
    if (pointer_array == NULL) {
        pointer_array = malloc(sizeof(void*));
    }else {
        pointer_array = realloc(pointer_array, sizeof(void*)*pointer_array_length);
    }
    pointer_array[pointer_array_length-1] = ptr;

    mem_array_length++;
    if (mem_array == NULL) {
        mem_array = malloc(sizeof (DbgMemInfo));
    }else {
        mem_array = realloc(mem_array, sizeof (DbgMemInfo)*mem_array_length);
    }
    DbgMemInfo info;
    info.file = file;
    info.line = line;
    info.freed = false;
    mem_array[mem_array_length-1] = info;

    DbgMemInfo* m = mem_array;
    int m2 = mem_array_length;
    return ptr;
}

void* dbg_realloc(void* ptr, size_t size, char* file, int line) {
    void* newPtr = realloc(ptr, size);
    if (newPtr == ptr) {
        return newPtr;
    }  

    for (int i = 0; i < pointer_array_length; i++) {
        if (pointer_array[i] == ptr) {
            pointer_array[i] = newPtr;
            break;
        }
    }

    return newPtr;
}

void dbg_free(void* ptr, char* file, int line) {
    for (int i = 0; i < pointer_array_length; i++) {
        if (pointer_array[i] == ptr) {
            mem_array[i].freed = true;
            break;
        }
    }
    free(ptr);
}

void dbg_free_meminfo() {
    if (mem_array == NULL) {
        free(mem_array);
        free(pointer_array);
    }
}