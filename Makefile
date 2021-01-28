all: build run-test

build: test.cpp
	g++ -g -o test -pg -std=c++14 test.cpp -lsqlite3

run-test: test
	time ./test

clean:
	rm -f test

.PHONY: run-test