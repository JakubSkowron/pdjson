.POSIX:
CC     = cc
CXX    = c++
CFLAGS = -std=c99 -pedantic -Wall -Wextra -Wno-missing-field-initializers
CXXFLAGS = -std=c++14 -Wall -Werror

all: tests/cpp-tests tests/pretty tests/stream tests/tests

tests/cpp-tests: tests/cpp-tests.o pdjson.o
	$(CXX) $(LDFLAGS) -o $@ tests/cpp-tests.o pdjson.o $(LDLIBS)

tests/pretty: tests/pretty.o pdjson-c.o
	$(CC) $(LDFLAGS) -o $@ tests/pretty.o pdjson-c.o $(LDLIBS)

tests/tests: tests/tests.o pdjson-c.o
	$(CC) $(LDFLAGS) -o $@ tests/tests.o pdjson-c.o $(LDLIBS)

tests/stream: tests/stream.o pdjson-c.o
	$(CC) $(LDFLAGS) -o $@ tests/stream.o pdjson-c.o $(LDLIBS)

pdjson.o: pdjson.cpp pdjson.h
tests/cpp-tests.o: tests/cpp-tests.cpp pdjson.h
tests/pretty.o: tests/pretty.c pdjson.h
tests/tests.o: tests/tests.c pdjson.h
tests/stream.o: tests/stream.c pdjson.h

pdjson-c.o: pdjson.cpp pdjson.h
	$(CC) -x c -c $(CFLAGS) -o pdjson-c.o pdjson.cpp

test: check
check: tests/tests
	tests/tests

clean:
	rm -f tests/cpp-tests tests/pretty tests/tests tests/stream
	rm -f pdjson.o tests/pretty.o tests/tests.o tests/stream.o pdjson-c.o

.c:
	$(CC) -c $(CFLAGS) -o $@ $<

.cpp:
	$(CXX) -c $(CXXFLAGS) -o $@ $<
