#ifndef PDJSON_PARSER_H
#define PDJSON_PARSER_H

#include <cstdio>

namespace pdjson {

enum class token_type {
    error = 1, done,
    object_start, object_end, array_start, array_end,
    string, number, json_true, json_false, json_null
};

namespace impl {

typedef token_type json_type;
typedef int (*json_user_io) (void *user);

struct json_source {
    int (*get) (struct json_source *);
    int (*peek) (struct json_source *);
    size_t position;
    union {
        struct {
            FILE *stream;
        } stream;
        struct {
            const char *buffer;
            size_t length;
        } buffer;
        struct {
            void *ptr;
            json_user_io get;
            json_user_io peek;
        } user;
    } source;
};

struct json_allocator {
    void *(*malloc)(size_t);
    void *(*realloc)(void *, size_t);
    void (*free)(void *);
};

struct json_stream {
    size_t lineno;

    struct json_stack *stack;
    size_t stack_top;
    size_t stack_size;
    json_type next;
    unsigned flags;

    struct {
        char *string;
        size_t string_fill;
        size_t string_size;
    } data;

    size_t ntokens;

    struct json_source source;
    struct json_allocator alloc;
    char errmsg[128];
};

}  // namespace impl

class tokenizer {
public:
  typedef int (*user_io) (void *user);

  tokenizer(const void* buffer, size_t size);
  tokenizer(const char* string);
  tokenizer(FILE* file);
  tokenizer(user_io get, user_io peek, void *user);
  ~tokenizer();

  token_type next();
private:
  impl::json_stream stream;
};

}  // namespace pdjson

#endif  // PDJSON_PARSER_H
