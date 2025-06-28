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
void _strbuf_write_string(StrBuf* buf, ...);
void strbuf_reset(StrBuf* buf);
void strbuf_free_contents(StrBuf* buf);

#define strbuf_write_string(buf, ...) _strbuf_write_string(buf, __VA_ARGS__, NULL)
#define dfprintf(stream, ...) printf(stream, "DEBUG: " ## __VA_ARGS__## "c")

#endif