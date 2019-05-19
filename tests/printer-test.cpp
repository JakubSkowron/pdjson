#include <cstdio>
#include "../printer.h"

int main( int argc, char* argv[] ) {
  // printing

  const char* expected = u8"{\"a\":-0.2,\"b\":100,\"😍\":{\"chrząszcz\":\"[ˈxʂɔw̃ʂt͡ʂ]\",\"a\":[1,2,[],3]}"
      ",\"\\n\":\"\\u000b\\u007f\\\\0\",\"c\":18446744073709551615,\"d\":255,\"e\":-128}";

  pdjson::printer json;
  json.new_object();
    json.number( "a", -0.2 );
    json.number( "b", 100 );
    json.new_object( u8"😍" );
      json.string( u8"chrząszcz", u8"[ˈxʂɔw̃ʂt͡ʂ]" );
      json.new_array("a");
        json.number(1);
        json.number(2);
        json.new_array();
        json.end_array();
        json.number(3);
      json.end_array();
    json.end_object();
    json.string("\n","\v\x7f\\0");
    json.number( "c", (std::uint64_t)-1 );
    json.number( "d", (unsigned char)-1 );
    json.number( "e", (signed char)-128 );
  json.end_object();

  if( json.str() != expected ) {
    std::puts("FAIL");
    std::puts("expected:");
    std::puts(expected);
    std::puts("result:");
    std::puts(json.str().c_str());
    return 1;
  }

  std::puts("PASS");
  // TODO: parse the string
}
