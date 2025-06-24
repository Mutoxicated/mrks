#ifndef HELPERS
#define HELPERS

typedef struct{
    char* array;
    int length;
} StrBuf;

StrBuf* NewStrBuf();
void AddChar(StrBuf* buf, char c);
void ResetStrBuf(StrBuf* buf);
char* String(StrBuf* buf);
void FreeStrBuf(StrBuf* buf);

typedef struct{
    int min;
    int max;
} Range;

Range NewRange(int min, int max);

#endif