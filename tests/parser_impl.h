#ifndef PDJSON_PARSER_IMPL_H
#define PDJSON_PARSER_IMPL_H

#include "../parser.h"

// Copied from parser.cpp

namespace pdjson {
namespace impl {

void json_open_buffer(json_stream *json, const void *buffer, size_t size);
void json_open_string(json_stream *json, const char *string);
void json_open_stream(json_stream *json, FILE *stream);
void json_open_user(json_stream *json, json_user_io get, json_user_io peek, void *user);
void json_close(json_stream *json);

void json_set_allocator(json_stream *json, json_allocator *a);
void json_set_streaming(json_stream *json, bool strict);

json_type json_next(json_stream *json);
json_type json_peek(json_stream *json);
void json_reset(json_stream *json);
const char *json_get_string(json_stream *json, size_t *length);
double json_get_number(json_stream *json);

json_type json_skip(json_stream *json);
json_type json_skip_until(json_stream *json, json_type type);

size_t json_get_lineno(json_stream *json);
size_t json_get_position(json_stream *json);
size_t json_get_depth(json_stream *json);
const char *json_get_error(json_stream *json);

}  // namespace impl
}  // namespace pdjson

#endif  // PDJSON_PARSER_IMPL_H
