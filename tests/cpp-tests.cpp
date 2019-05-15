#include "../pdjson.h"

#include <cstdio>

int main( int argc, char* argv[] ) {
  const char* str = argv[1] ? argv[1] : "{ \"first\": [1,2,3,4.5], \"second\": {}}";
  pdjson::parser parser(str);
  
  for( pdjson::value v = parser.next(); v.base.type != pdjson::type::end; v = parser.next() ) {
    std::printf( "type == %d\n", (int)v.base.type );
  }
  std::printf("end\n");
}
