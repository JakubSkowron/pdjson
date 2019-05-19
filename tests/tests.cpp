#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "parser_impl.h"

#if _WIN32
#  define C_RED(s)   s
#  define C_GREEN(s) s
#  define C_BOLD(s)  s
#else
#  define C_RED(s)   "\033[31;1m" s "\033[0m"
#  define C_GREEN(s) "\033[32;1m" s "\033[0m"
#  define C_BOLD(s)  "\033[1m"    s "\033[0m"
#endif

using namespace pdjson::impl;

struct expect {
    json_type type;
    const char *str;
};

#define countof(a) (sizeof(a) / sizeof(*a))

#define TEST(name, stream) \
    do { \
        int r = test(name, stream, seq, countof(seq), str, sizeof(str) - 1); \
        if (r) \
            count_pass++; \
        else \
            count_fail++; \
    } while (0)

const char* json_typename[] = {
    "", "ERROR", "DONE", "OBJECT", "OBJECT_END", "ARRAY", "ARRAY_END", "STRING", "NUMBER", "TRUE", "FALSE", "NULL"};

static int
has_value(json_type type)
{
    return type == json_type::string || type == json_type::number;
}

static int
test(const char *name,
     int stream,
     struct expect *seq,
     size_t seqlen,
     const char *buf,
     size_t len)
{
    int success = 1;
    struct json_stream json[1];
    json_type expect, actual;
    const char *expect_str, *actual_str;

    json_open_buffer(json, buf, len);
    json_set_streaming(json, stream);
    for (size_t i = 0; success && i < seqlen; i++) {
        expect = seq[i].type;
        actual = json_next(json);
        actual_str = has_value(actual) ? json_get_string(json, 0) : "";
        expect_str = seq[i].str ? seq[i].str : "";

        if (actual != expect)
            success = 0;
        else if (seq[i].str && !!strcmp(expect_str, actual_str))
            success = 0;
        else if (stream && actual == json_type::done)
            json_reset(json);
    }

    if (success) {
        printf(C_GREEN("PASS") " %s\n", name);
    } else {
        printf(C_RED("FAIL") " %s: "
               "expect " C_BOLD("%s") " %s / "
               "actual " C_BOLD("%s") " %s\n",
               name,
               json_typename[(int)expect], expect_str,
               json_typename[(int)actual], actual_str);
    }
    json_close(json);
    return success;
}

int
main(void)
{
    int count_pass = 0;
    int count_fail = 0;

    {
        const char str[] = "  1024\n";
        struct expect seq[] = {
            {json_type::number, "1024"},
            {json_type::done},
        };
        TEST("number", false);
    }

    {
        const char str[] = "  true \n";
        struct expect seq[] = {
            {json_type::json_true},
            {json_type::done},
        };
        TEST("true", false);
    }

    {
        const char str[] = "\nfalse\r\n";
        struct expect seq[] = {
            {json_type::json_false},
            {json_type::done},
        };
        TEST("false", false);
    }

    {
        const char str[] = "\tnull";
        struct expect seq[] = {
            {json_type::json_null},
            {json_type::done},
        };
        TEST("null", false);
    }

    {
        const char str[] = "\"foo\"";
        struct expect seq[] = {
            {json_type::string, "foo"},
            {json_type::done},
        };
        TEST("string", false);
    }

    {
        const char str[] = "\"Tim \\\"The Tool Man\\\" Taylor\"";
        struct expect seq[] = {
            {json_type::string, "Tim \"The Tool Man\" Taylor"},
            {json_type::done},
        };
        TEST("string quotes", false);
    }

    {
        const char str[] = "{\"abc\": -1}";
        struct expect seq[] = {
            {json_type::object_start},
            {json_type::string, "abc"},
            {json_type::number, "-1"},
            {json_type::object_end},
            {json_type::done},
        };
        TEST("object", false);
    }

    {
        const char str[] = "[1, \"two\", true, null]";
        struct expect seq[] = {
            {json_type::array_start},
            {json_type::number, "1"},
            {json_type::string, "two"},
            {json_type::json_true},
            {json_type::json_null},
            {json_type::array_end},
            {json_type::done},
        };
        TEST("array", false);
    }

    {
        const char str[] = "1 10 100 2002";
        struct expect seq[] = {
            {json_type::number, "1"},
            {json_type::done},
            {json_type::number, "10"},
            {json_type::done},
            {json_type::number, "100"},
            {json_type::done},
            {json_type::number, "2002"},
            {json_type::done},
            {json_type::error},
        };
        TEST("number stream", true);
    }

    {
        const char str[] = "{\"foo\": [1, 2, 3]}\n[]\n\"name\"";
        struct expect seq[] = {
            {json_type::object_start},
            {json_type::string, "foo"},
            {json_type::array_start},
            {json_type::number, "1"},
            {json_type::number, "2"},
            {json_type::number, "3"},
            {json_type::array_end},
            {json_type::object_end},
            {json_type::done},
            {json_type::array_start},
            {json_type::array_end},
            {json_type::done},
            {json_type::string, "name"},
            {json_type::done},
            {json_type::error},
        };
        TEST("mixed stream", true);
    }

    {
        const char str[] = "[1, 2, 3";
        struct expect seq[] = {
            {json_type::array_start},
            {json_type::number, "1"},
            {json_type::number, "2"},
            {json_type::number, "3"},
            {json_type::error},
        };
        TEST("incomplete array", false);
    }

    {
        const char str[] = "\"\\u0068\\u0065\\u006c\\u006c\\u006F\"";
        struct expect seq[] = {
            {json_type::string, "hello"},
            {json_type::done},
        };
        TEST("\\uXXXX", false);
    }

    {
        /* This surrogate half must precede another half */
        const char str[] = "\"\\uD800\\u0065\"";
        struct expect seq[] = {
            {json_type::error}
        };
        TEST("invalid surrogate pair", false);
    }

    {
        /* This surrogate half cannot be alone */
        const char str[] = "\"\\uDC00\"";
        struct expect seq[] = {
            {json_type::error}
        };
        TEST("invalid surrogate half", false);
    }

    {
        /* Surrogate halves are in the wrong order */
        const char str[] = "\":\\uDc00\\uD800\"";
        struct expect seq[] = {
            {json_type::error}
        };
        TEST("surrogate misorder", false);
    }

    {
        /* This is a valid encoding for U+10000 */
        const char str[] = "\":\\uD800\\uDC00\"";
        struct expect seq[] = {
            {json_type::string, ":\xf0\x90\x80\x80"}, /* UTF-8 for U+10000 */
            {json_type::done},
        };
        TEST("surrogate pair", false);
    }

    printf("%d pass, %d fail\n", count_pass, count_fail);
    exit(count_fail ? EXIT_FAILURE : EXIT_SUCCESS);
}
