.POSIX:
CXX    = c++
CXXFLAGS = -std=c++14 -Wall -Werror

all: tests/cpp-tests tests/pretty tests/stream tests/tests

tests/cpp-tests: tests/cpp-tests.o pdjson.o
	$(CXX) $(LDFLAGS) -o $@ tests/cpp-tests.o pdjson.o $(LDLIBS)

tests/pretty: tests/pretty.o pdjson.o
	$(CXX) $(LDFLAGS) -o $@ tests/pretty.o pdjson.o $(LDLIBS)

tests/tests: tests/tests.o pdjson.o
	$(CXX) $(LDFLAGS) -o $@ tests/tests.o pdjson.o $(LDLIBS)

tests/stream: tests/stream.o pdjson.o
	$(CXX) $(LDFLAGS) -o $@ tests/stream.o pdjson.o $(LDLIBS)

pdjson.o: pdjson.cpp parser.h printer.h
tests/cpp-tests.o: tests/cpp-tests.cpp parser.h printer.h
tests/pretty.o: tests/pretty.cpp parser.h
tests/tests.o: tests/tests.cpp parser.h
tests/stream.o: tests/stream.cpp parser.h

test: check
check: tests/tests
	tests/tests

clean:
	rm -f tests/cpp-tests tests/pretty tests/tests tests/stream
	rm -f pdjson.o tests/pretty.o tests/tests.o tests/stream.o

.cpp:
	$(CXX) -c $(CXXFLAGS) -o $@ $<
