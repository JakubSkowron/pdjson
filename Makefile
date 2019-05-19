.POSIX:
CXX = c++
CXXFLAGS = -std=c++14 -Wall -Werror

all: test tests/pretty tests/stream

tests/printer-test: tests/printer-test.o printer.o
	$(CXX) $(LDFLAGS) -o $@ tests/printer-test.o printer.o $(LDLIBS)

tests/pretty: tests/pretty.o parser.o
	$(CXX) $(LDFLAGS) -o $@ tests/pretty.o parser.o $(LDLIBS)

tests/tests: tests/tests.o parser.o
	$(CXX) $(LDFLAGS) -o $@ tests/tests.o parser.o $(LDLIBS)

tests/stream: tests/stream.o parser.o
	$(CXX) $(LDFLAGS) -o $@ tests/stream.o parser.o $(LDLIBS)

parser.o: parser.cpp parser.h
printer.o: printer.cpp printer.h
tests/printer-test.o: tests/printer-test.cpp parser.h printer.h
tests/pretty.o: tests/pretty.cpp parser.h
tests/tests.o: tests/tests.cpp parser.h
tests/stream.o: tests/stream.cpp parser.h

test: check
check: tests/tests tests/printer-test
	tests/tests
	tests/printer-test

clean:
	rm -f tests/printer-test tests/pretty tests/tests tests/stream
	rm -f parser.o printer.o tests/printer-test.o tests/pretty.o tests/tests.o tests/stream.o

.cpp:
	$(CXX) -c $(CXXFLAGS) -o $@ $<
