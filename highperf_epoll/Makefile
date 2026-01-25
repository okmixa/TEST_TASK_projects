CC = gcc
CXX = g++
CFLAGS = -O2 -std=c11 -Wall -Wextra -D_GNU_SOURCE -Iinclude
CXXFLAGS = -O2 -std=c++17 -Iinclude -Ideps/asio/include

build:
	mkdir -p build

server: build src/server.c src/parser.cpp
	$(CC) $(CFLAGS) -c src/server.c -o build/server_c.o
	$(CXX) $(CXXFLAGS) -c src/parser.cpp -o build/parser.o
	$(CXX) build/server_c.o build/parser.o -o server

client: client_main.cpp
	mkdir -p build
	$(CXX) $(CXXFLAGS) client_main.cpp -lpthread -o client

stress_test: tests/stress_test.cpp
	mkdir -p build
	$(CXX) $(CXXFLAGS) -I. -Ideps/asio/include tests/stress_test.cpp -lpthread -o stress_test

build:
	mkdir -p build

clean:
	rm -rf server client stress_test build/

.PHONY: clean
