#include <cstdio>
#include "../printer.h"

int main( int argc, char* argv[] ) {
  // printing

  pdjson::printer json;
  json.new_object();
    json.number( "a", -0.2 );
    json.number( "b", 100 );
    json.new_object( u8"😍" );
      json.string( u8"chrząszcz brzmi w trzcinie", u8"[ˈxʂɔw̃ʂt͡ʂ ˈbʐmi fˈtʂt͡ɕiɲɛ]" );
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

  std::puts( json.str().c_str() );

  // TODO: parse the string
}
