#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include "strbuf.h"
#include "tomlc17.h"
#include "dbg_options.h"

#define RED "\x1b[31m"

void reverse(char str[], int length) {
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        end--;
        start++;
    }
}

char* itoa(StrBuf* buf, int num, int base) {
    bool isNegative = false;

    if (num == 0) {
        strbuf_write(buf, '0');
        goto exit;
    }

    // In standard itoa(), negative numbers are handled
    // only with base 10. Otherwise numbers are
    // considered unsigned.
    if (num < 0 && base == 10) {
        isNegative = true;
        num = -num;
    }

    while (num != 0) {
        int rem = num % base;
        strbuf_write(buf, rem > 9 ? (rem - 10) + 'a' : rem + '0');
        num = num / base;
    }

    if (isNegative)
        strbuf_write(buf, '-');

    reverse(buf->array, buf->length);
    exit:
    return buf->array;
}


static const char* path_prepend = "./src/";
static const char* filename = "/utils/macros.toml";

int main(int argc, char const *argv[]) {
    char cwd[200];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working dir: %s\n", cwd);
    } else {
        perror(RED"getcwd() error");
        return 1;
    }
    char macro_dest[strlen(cwd)+strlen(filename)+1]; 
    strcpy(macro_dest, cwd);
    strcat(macro_dest, filename);
    toml_result_t result = toml_parse_file_ex(macro_dest);

    if (!result.ok) {
        printf("%s\n", result.errmsg);
        goto exit;
    }

    toml_datum_t macros = toml_seek(result.toptab, "macros");
    if (macros.type != TOML_ARRAY) {
        return 0;
    }
    printf("->Going through macros...\n");
    for (int i = 0; i < macros.u.arr.size; i++) {
        toml_datum_t mcro = macros.u.arr.elem[i];
        if (mcro.type != TOML_TABLE) {
            printf(RED"Macro was not a table\n");
            goto exit;
        }
        if (*mcro.u.tab.len < 3) {
            printf(RED"Macro must have 3 parameters.\n");
            goto exit;
        }
        if (strcmp(mcro.u.tab.key[0], "name") != 0) {
            printf(RED"First macro table key must be 'name'.\n");
            goto exit;
        }
        if (mcro.u.tab.value[0].type != TOML_STRING) {
            printf(RED"'Name' must be a string.\n");
            goto exit;
        }
        if (strcmp(mcro.u.tab.key[1], "params") != 0) {
            printf(RED"Second macro table key must be 'params'.\n");
            goto exit;
        }
        if (mcro.u.tab.value[1].type != TOML_ARRAY) {
            printf(RED"'Params' must be an array.\n");
            goto exit;
        }
        if (strcmp(mcro.u.tab.key[2], "source") != 0) {
            printf(RED"Third macro table key must be 'source'.\n");
            goto exit;
        }
        if (mcro.u.tab.value[2].type != TOML_STRING) {
            printf(RED"'Source' must be a string.\n");
            goto exit;
        }
    }
    printf("Macros done\n->Going through invocations...\n");
    toml_datum_t invocations = toml_seek(result.toptab, "invocations");
    if (invocations.type != TOML_ARRAY) {
        printf(RED"Invocations must be an array.\n");
        goto exit;
    }
    for (int i = 0; i < invocations.u.arr.size; i++) {
        printf("----------------------------- INVOCATION %d\n", i+1);
        toml_datum_t invocation = invocations.u.arr.elem[i];
        if (invocation.type != TOML_TABLE) {
            printf(RED"Invocation was not a table\n");
            goto exit;
        }

        if (strcmp(invocation.u.tab.key[0], "name") != 0) {
            printf(RED"First invocation table key must be 'name'.\n");
            goto exit;
        }
        if (invocation.u.tab.value[0].type != TOML_STRING) {
            printf(RED"'Name' must be a string.\n");
            goto exit;
        }

        if (strcmp(invocation.u.tab.key[1], "args") != 0) {
            printf(RED"Second invocation table key must be 'args'.\n");
            goto exit;
        }
        if (invocation.u.tab.value[1].type != TOML_ARRAY) {
            printf(RED"'Args' must be an array.\n");
            goto exit;
        }

        if (strcmp(invocation.u.tab.key[2], "destination") != 0) {
            printf(RED"Third invocation table key must be 'destination'.\n");
            goto exit;
        }
        if (invocation.u.tab.value[2].type != TOML_STRING) {
            printf(RED"'Destination' must be a string\n");
            goto exit;
        }

        if (strcmp(invocation.u.tab.key[3], "flag") != 0) {
            printf(RED"Fourth invocation table key must be 'flag'.\n");
            goto exit;
        }
        if (invocation.u.tab.value[3].type != TOML_INT64) {
            printf(RED"'Flag' must be an int.\n");
            goto exit;
        }

        toml_datum_t macro_name = invocation.u.tab.value[0];
        printf("->Finding macro '%s'\n", macro_name.u.s);

        toml_datum_t macro_found;
        bool found_macro  = false;
        for (int k = 0; k < macros.u.arr.size; k++) {
            toml_datum_t mcro = macros.u.arr.elem[k];
            if (strcmp(mcro.u.tab.value[0].u.s, macro_name.u.s) != 0) {
                continue;
            }
            macro_found = mcro;
            found_macro = true;
            break;
        }
        if (!found_macro) {
            printf(RED"No macro named %s found\n", macro_name.u.s);
            goto exit;
        }
        printf("Macro '%s' found\n\n->Creating new source instance...\n", macro_name.u.s);

        /* Replace all macro parameters in the source with the invocation args */
        const char* source = macro_found.u.tab.value[2].u.s;
        int source_length = strlen(source);

        StrBuf _buf = strbuf_new_cap(source_length);
        StrBuf* buf = &_buf;
        StrBuf _macro_param_name = strbuf_new();
        StrBuf* macro_param_name = &_macro_param_name;

        bool at_macro_param = false;
        for (int j = 0; j < source_length; j++) {
            if (at_macro_param && !isalpha(source[j])) {
                at_macro_param = false;
                char* source_param = _macro_param_name.array;
                toml_datum_t macro_found_params = macro_found.u.tab.value[1];
                for (int k = 0; k < macro_found_params.u.arr.size; k++) {
                    const char* param = macro_found_params.u.arr.elem[k].u.s;
                    if (strcmp(param, source_param) == 0) {
                        strbuf_write_string(buf, invocation.u.tab.value[1].u.arr.elem[k].u.s);
                        break;
                    }
                }
                strbuf_reset(macro_param_name);
            }else if (at_macro_param) {
                strbuf_write(macro_param_name, source[j]);
                continue;
            }
            if (source[j] == '#' && j+1 < source_length && source[j+1 == '#']) {
                j++;
                continue;
            }
            if (source[j] == '$') {
                at_macro_param = true;
                continue;
            }
            strbuf_write(buf, source[j]);
        }
        strbuf_free_contents(macro_param_name);

        /* Print the buf to the location defined from destination and flag */
        const char* destination = invocation.u.tab.value[2].u.s;
        char full_destination[strlen(destination)+strlen(cwd)];
        strcpy(full_destination, cwd);
        strcat(full_destination, destination);

        printf("Source instance created\n\n->Opening file destination (%s)\n", full_destination);
        FILE* file = fopen(full_destination, "r+");
        if (file == NULL) {
            printf(RED"File could not be opened.\n");
            strbuf_free_contents(buf);
            goto exit;
        }

        fseek(file, 0L, SEEK_END);
        int pos = ftell(file);
        fseek(file, 0L, SEEK_SET);

        StrBuf _file_buffer = strbuf_new_cap(pos);
        StrBuf* file_buffer = &_file_buffer;
        StrBuf _comment_buffer = strbuf_new_cap(4);
        StrBuf* comment_buffer = &_comment_buffer;
        bool prev_char_was_slash = false;
        bool in_comment = false;
        bool is_flag_comment = false;
        int flag = -1;
        bool flag_update = false;
        int flag_update_char_jump = -1;
        long passed_chars = 0;
        int expected_flag = invocation.u.tab.value[3].u.int64;

        printf("File opened\n->Searching for flag (%d)...\n", expected_flag);
        char c;
        while (true) {
            c = fgetc(file);
            if (c == EOF) {
                break;
            }
            if (flag_update_char_jump > 0) {
                flag_update_char_jump--;
                continue;
            }
            if (is_flag_comment) {
                passed_chars += 1;
                bool is_whitespace = (c == '\n' || c == ' ');
                if (!is_whitespace) {
                    if (flag == -1 && !isdigit(c)) {
                        printf(RED"Flag must be a number (file: %s) (got %s%c)\n", full_destination, _comment_buffer.array, c);
                        strbuf_free_contents(comment_buffer);
                        strbuf_free_contents(buf);
                        strbuf_free_contents(file_buffer);
                        goto exit;
                    }
                    strbuf_write(comment_buffer, c);
                    continue;
                }
                if (flag == -1) {
                    if (_comment_buffer.length == 0) {
                        printf(RED"Flag must be a number (file: %s) (got nothing)\n", full_destination);
                        strbuf_free_contents(comment_buffer);
                        strbuf_free_contents(buf);
                        strbuf_free_contents(file_buffer);
                        goto exit;
                    }
                    int flg = atoi(_comment_buffer.array);
                    if (flg != expected_flag) {
                        is_flag_comment = false;
                        in_comment = false;
                        printf("Not the right flag (%d != %d)\n", flg, expected_flag);
                        fseek(file, -passed_chars, SEEK_CUR);
                        passed_chars = 0;
                        strbuf_reset(comment_buffer);
                        continue;
                    }
                    flag = flg;
                }else if (strcmp(_comment_buffer.array, "D") == 0) {
                    flag_update = true;
                }else if (flag_update) {
                    flag_update_char_jump = atoi(_comment_buffer.array);
                }
                strbuf_reset(comment_buffer);
                if (c != '\n') {
                    continue;
                }
                printf("Flag %d found! Flag update jump: %d. Writing source instance.\n", flag, flag_update_char_jump);
                strbuf_write_string(file_buffer, " FLAG ");
                StrBuf num = strbuf_new();
                itoa(&num, flag, 10);
                strbuf_write_string(file_buffer, num.array, " D ");
                strbuf_reset(&num);
                if (flag_update_char_jump == -1) {
                    printf("Buf length: %d\n", _buf.length);
                }
                itoa(&num, _buf.length, 10);
                strbuf_write_string(file_buffer, num.array);
                strbuf_free_contents(&num);
                strbuf_write(file_buffer, c);

                strbuf_write_string(file_buffer, _buf.array);
                if (!flag_update) {
                    strbuf_write_string(file_buffer, "// END: DON'T MANIPULATE THIS AREA!\n");
                }
                flag = -1;
                passed_chars = 0;
                is_flag_comment = false;
                in_comment = false;
                continue;
            }
            if (in_comment) {
                passed_chars += 1;
                if (c == '\n') {
                    in_comment = false;
                    fseek(file, -passed_chars, SEEK_CUR);
                    passed_chars = 0;
                    strbuf_reset(comment_buffer);
                    continue;
                }
                if (c != ' ') {
                    strbuf_write(comment_buffer, c);
                    continue;
                }
                if (strcmp(_comment_buffer.array, "FLAG") == 0) {
                    is_flag_comment = true;
                    strbuf_reset(comment_buffer);
                    continue;
                }
                strbuf_reset(comment_buffer);
                continue;
            }
            strbuf_write(file_buffer, c);
            switch (c) {
                case '\t': continue;
                case '\r': continue;
            }
            if (c == '/' && prev_char_was_slash == false) {
                prev_char_was_slash = true;
                continue;
            }
            if (c == '/' && prev_char_was_slash == true) {
                in_comment = true;
                prev_char_was_slash = false;
                continue;
            }
            if (c != '/' && prev_char_was_slash == true) {
                prev_char_was_slash = false;
            }
        }
        fseek(file, 0L, SEEK_SET);
        fputs(_file_buffer.array, file);
        ftruncate(fileno(file), _file_buffer.length);
        fclose(file);
        strbuf_free_contents(comment_buffer);
        strbuf_free_contents(buf);
        strbuf_free_contents(file_buffer);
        if (i != invocations.u.arr.size-1) {
            printf("\n");
        }
    }
    printf("\n*** Successfully expanded all macro invocations! ***\n");
    exit:
    toml_free(result);

    #if MEM_DBG_ENABLED 
    dbg_free_meminfo();
    dbg_write_meminfo();
    #endif
    return 0;
}   
