#ifndef DBG_OPTIONS
#define DBG_OPTIONS
#define MEM_DBG_ENABLED true

#if MEM_DBG_ENABLED == true
#include "dbg_internal.h"
#define malloc(A) dbg_malloc(A, __FILE__, __LINE__);
#define realloc(A, B) dbg_realloc(A, B, __FILE__, __LINE__);
#define free(A) dbg_free(A, __FILE__, __LINE__);
#endif

#endif