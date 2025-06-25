#include "helpers.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "tomlc17.h"

static const char* path_prepend = "./src/";

int action(int argc, char const *argv[]) {
    toml_result_t result = toml_parse_file_ex("macros.toml");

    if (!result.ok) {
        printf("%s\n", result.errmsg);
        goto exit;
    }

    toml_datum_t macros = toml_seek(result.toptab, "macros");
    if (macros.type != TOML_ARRAY) {
        return 0;
    }
    for (int i = 0; i < macros.u.arr.size; i++) {
        toml_datum_t mcro = macros.u.arr.elem[i];
        if (mcro.type != TOML_TABLE) {
            printf("Macro was not a table\n");
            goto exit;
        }
        if (*mcro.u.tab.len != 3) {
            printf("Macro table must have a name, params and source\n");
            goto exit;
        }
    }
    toml_datum_t invocations = toml_seek(result.toptab, "invocations");
    if (invocations.type != TOML_ARRAY) {
        return 0;
    }
    for (int i = 0; i < invocations.u.arr.size; i++) {
        toml_datum_t invocation = invocations.u.arr.elem[i];
        if (invocation.type != TOML_TABLE) {
            printf("Invocation was not a table\n");
            goto exit;
        }
        if (*invocation.u.tab.len != 4) {
            printf("invocation table must have a name, args, destination and flag\n");
            goto exit;
        } 

        toml_datum_t macro_name = invocation.u.tab.value[0];
        
        toml_datum_t macro_found;
        bool found_macro  = false;
        for (int k = 0; k < macros.u.arr.size; k++) {
            toml_datum_t mcro = macros.u.arr.elem[i];
            if (mcro.u.tab.key[0] != macro_name.u.s) {
                continue;
            }
            macro_found = mcro;
            found_macro = true;
            break;
        }
        if (!found_macro) {
            printf("no macro named %s", macro_name.u.s);
            goto exit;
        }

        /* Replace all macro parameters in the source with the invocation args */
        StrBuf _buf = strbuf_new();
        StrBuf* buf = &_buf;
        StrBuf _macro_param_name = strbuf_new();
        StrBuf* macro_param_name = &_macro_param_name;

        bool at_macro_param = false;
        
        const char* source = macro_found.u.tab.value[1].u.s;
        int source_length = strlen(source);
        for (int j = 0; j < source_length; j++) {
            if (at_macro_param && !isalpha(source[j])) {
                at_macro_param = false;
                char* source_param = _macro_param_name.array;
                toml_datum_t macro_found_params = macro_found.u.tab.value[1];
                for (int k = 0; k < macro_found_params.u.arr.size; k++) {
                    char* param = macro_found_params.u.arr.elem[k].u.s;
                    if (param == source_param) {
                        strbuf_write_string(buf, invocation.u.tab.value[1].u.arr.elem[k].u.s, NULL);
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

        strbuf_free(macro_param_name);

        /* Print the buf to the location defined from destination and flag */

        char* destination = invocation.u.tab.value[2].u.s;
        char full_destination[strlen(destination)+strlen(path_prepend)+1];
        strcpy(full_destination, destination);
        strcat(full_destination, path_prepend);
        FILE* file = fopen(full_destination, "r+");
        if (file == NULL) {
            printf("File '%s' could not be opened.", full_destination);
            strbuf_free(buf);
            goto exit;
        }

        StrBuf _comment_buffer = strbuf_new();
        StrBuf* comment_buffer = &_comment_buffer;
        bool prev_char_was_slash = false;
        bool in_comment = false;
        bool is_flag_comment = false;
        int flag = -1;

        char c;
        while (true) {
            c = fgetc(file);
            if (c == EOF) {
                break;
            }
            switch (c) {
                case '\t': continue;
                case '\r': continue;
            }
            if (is_flag_comment) {
                bool is_whitespace = (c == '\n' || c == ' ');
                if (is_whitespace && strlen(_comment_buffer.array) != 0) {
                    int flag = atoi(_comment_buffer.array);
                    if (flag == invocation.u.tab.value[3].u.int64) {
                        fgetc(file);
                        fputs(_buf.array, file);
                        break;
                    }
                }
                if (!isdigit(c)) {
                    printf("Flag must be a number (file: %s)\n", full_destination);
                    strbuf_free(comment_buffer);
                    strbuf_free(buf);
                    goto exit;
                }
                strbuf_write(comment_buffer, c);
                continue;
            }
            if (in_comment) {
                if (c == '\n') {
                    in_comment = false;
                    strbuf_reset(comment_buffer);
                    continue;
                }
                if (c != ' ') {
                    strbuf_write(comment_buffer, c);
                    continue;
                }
                if (_comment_buffer.array == "FLAG") {
                    is_flag_comment = true;
                    strbuf_reset(comment_buffer);
                    continue;
                }
                strbuf_reset(comment_buffer);
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
    }

    exit:
    toml_free(result);
    return 0;
}   
