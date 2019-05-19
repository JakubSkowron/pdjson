/* This tool prints out the token stream for standard input. It's primarily
 * intended to help write tests.
 */
#include <stdio.h>

#include "parser_impl.h"

using namespace pdjson::impl;

const char* json_typename[] = {
    "", "ERROR", "DONE", "OBJECT", "OBJECT_END", "ARRAY", "ARRAY_END", "STRING", "NUMBER", "TRUE", "FALSE", "NULL"};

int
main(void)
{
    json_stream s[1];
    json_open_stream(s, stdin);
    json_set_streaming(s, 1);
    puts("struct expect seq[] = {");
    for (;;) {
        json_type type = json_next(s);
        const char *value = 0;
        switch (type) {
            case json_type::done:
                json_reset(s);
                break;
            case json_type::json_null:
                value = "null";
                break;
            case json_type::json_true:
                value = "true";
                break;
            case json_type::json_false:
                value = "false";
                break;
            case json_type::number:
                value = json_get_string(s, 0);
                break;
            case json_type::string:
                value = json_get_string(s, 0);
                break;
            case json_type::array_start:
            case json_type::object_start:
            case json_type::object_end:
            case json_type::array_end:
            case json_type::error:
                break;
        }
        if (value)
            printf("    {JSON_%s, \"%s\"},\n", json_typename[(int)type], value);
        else
            printf("    {JSON_%s},\n", json_typename[(int)type]);
        if (type == json_type::error)
            break;
    }
    puts("};");
    json_close(s);
}
