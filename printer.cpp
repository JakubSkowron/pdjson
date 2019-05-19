#include "printer.h"

namespace pdjson {

static std::string escape(const char* s) {
  std::string str;
  for(;*s;++s) {
    switch(*s) {
    case '"': str += "\\\""; break;
    case '\\': str += "\\\\"; break;
    case '\b': str += "\\b"; break;
    case '\f': str += "\\f"; break;
    case '\n': str += "\\n"; break;
    case '\r': str += "\\r"; break;
    case '\t': str += "\\t"; break;
    case '\x7f': str += "\\u007f"; break;
    default:
      if((unsigned char)*s < 0x20) {
        char buf[7];
        std::snprintf( buf, sizeof(buf), "\\u%04x", (unsigned char)*s );
        str += buf;
      } else {
        str += *s;
      }
    }
  }
  return str;
}

void printer::put_comma() {
  if( need_comma ) {
    ss << ',';
    need_comma = false;
  }
}

void printer::new_key(const char* key) {
  put_comma();
  ss << '"' << escape(key) << '"' << ':';
  need_comma = false;
}

void printer::new_object() {
  put_comma();
  ss << '{';
  need_comma = false;
}

void printer::end_object() {
  ss << '}';
  need_comma = true;
}

void printer::new_array() {
  put_comma();
  ss << '[';
  need_comma = false;
}
void printer::end_array() {
  ss << ']';
  need_comma = true;
}

void printer::boolean(bool b) {
  put_comma();
  ss << (b ? "true" : "false");
  need_comma = true;
}

void printer::string(const char* s) {
  put_comma();
  ss << '"' << escape(s) << '"';
  need_comma = true;
}

void printer::null() {
  put_comma();
  ss << "null";
  need_comma = true;
}

}  // namespace pdjson
