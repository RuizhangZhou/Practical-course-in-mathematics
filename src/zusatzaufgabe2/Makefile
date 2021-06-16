CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -O3 -g

.PHONY: clean

run: main.out
	./main.out

all: main.out

main.out: a3.o a3_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^

a3.o: a3.cpp a3.h
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f a3.o main.out
