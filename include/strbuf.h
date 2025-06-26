#ifndef STRBUF
#define STRBUF

typedef struct{
    char* array;
    int length;
    int cap;
} StrBuf;

StrBuf strbuf_new();
StrBuf strbuf_new_cap(int cap);
void strbuf_write(StrBuf* buf, char c);
void strbuf_write_string(StrBuf* buf, ...);
void strbuf_reset(StrBuf* buf);
void strbuf_free(StrBuf* buf);
char* strbuf_get_str(StrBuf* buf);

#endif