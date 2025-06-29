#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "dbg_internal.h"
#include "strbuf.h"
#include "color.h"

void** pointer_array = NULL;
int pointer_array_length = 0;
DbgMemInfo* mem_array = NULL;
int mem_array_length = 0;

void* dbg_malloc(size_t size, char* file, int line) {
    void* ptr = malloc(size+sizeof(int));

    int associated_ptr = -1;
    for (int i = pointer_array_length-1; i >= 0; i--) {
        if (pointer_array[i] != ptr) {
            continue;
        }
        associated_ptr = i;
        if (!mem_array[i].freed) {
            printf(BOLD_WHITE()RED()"MEMORY IS BEING OVERWRITTEN\n");
            exit(-1);
        }
        break;
    }
    pointer_array_length++;
    if (pointer_array == NULL) {
        pointer_array = (void**)malloc(sizeof(void*));
    }else {
        pointer_array = (void**)realloc(pointer_array, sizeof(void*)*pointer_array_length);
    }
    pointer_array[pointer_array_length-1] = ptr;

    mem_array_length++;
    if (mem_array == NULL) {
        mem_array = (DbgMemInfo*)malloc(sizeof (DbgMemInfo));
    }else {
        mem_array = (DbgMemInfo*)realloc(mem_array, sizeof (DbgMemInfo)*mem_array_length);
    }
    DbgMemInfo info;
    info.file = file;
    info.line = line;
    info.freed = false;
    info.reallocs = 0;
    info.size_total = size;
    info.associated_ptr = associated_ptr;
    mem_array[mem_array_length-1] = info;
    return ptr;
}

void* dbg_realloc(void* ptr, size_t size, char* file, int line) {
    void* newPtr = realloc(ptr, size);

    for (int i = pointer_array_length-1; i >= 0; i--) {
        if (pointer_array[i] == ptr) {
            DbgMemInfo* a = mem_array+i;
            a->reallocs++;
            a->size_total += size;

            if (ptr != newPtr) {
                pointer_array[i] = newPtr;
            }
            break;
        }
    }

    return newPtr;
}

void dbg_free(void* ptr, char* file, int line) {
    for (int i = pointer_array_length-1; i >= 0; i--) {
        if (pointer_array[i] == ptr) {
            if (mem_array[i].freed) {
                printf(BOLD_WHITE()RED()"DOUBLE FREE DETECTED\n" WHITE());
                exit(-1);
            }
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

void dbg_write_meminfo() {
    if (mem_array == NULL) {
        return;
    }
    DbgMemInfo dbg_mem_info;
    FILE* file = fopen("mem_dbg_info.txt", "w");
    for (int i = 0; i < mem_array_length; i++) {
        dbg_mem_info = mem_array[i];
        fprintf(file, 
            "Allocation #%d (%p, %d):\n\tFile: %s\n\tLine: %d\n\tFreed: %s\n\tReallocs: %d\n\tTotalSize:%d\n", 
            i, pointer_array[i], dbg_mem_info.associated_ptr, dbg_mem_info.file, dbg_mem_info.line, dbg_mem_info.freed == 0 ? "false" : "true", 
            dbg_mem_info.reallocs, dbg_mem_info.size_total
        );
    }
    fclose(file);
}