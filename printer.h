#ifndef PDJSON_PRINTER_H
#define PDJSON_PRINTER_H

#include <type_traits>
#include <sstream>

namespace pdjson {

// JSON Printing

class printer {
public:
  void new_key(const char* key);

  void new_object();
  void new_object(const char* key) { new_key(key); new_object(); }
  void end_object();

  void new_array();
  void new_array(const char* key) { new_key(key); new_array(); }
  void end_array();

  void boolean(bool b);
  void boolean(const char* key, bool b) { new_key(key); boolean(b); }

  void string(const char* s);
  void string(const char* key, const char* s) { new_key(key); string(s); }

  void null();
  void null(const char* key) { new_key(key); null(); }

  template<typename T>
  void number(T t) {
    static_assert(std::is_arithmetic<T>::value);
    static_assert(!std::is_same<bool,T>::value);

    put_comma();
    ss << +t;  // char types needs conversion to printable integer, hence unary +
    need_comma = true;
  }
  template<typename T>
  void number(const char* key, T t) { new_key(key); number(t); }

  std::string str() const { return ss.str(); };

private:
  bool need_comma = false;
  void put_comma();
  std::ostringstream ss;
};

}  // namespace pdjson

#endif  // PDJSON_PRINTER_H
