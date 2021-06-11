CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -O3

.PHONY: clean

run: mapraview.out
	./mapraview.out

all: mapraview.out

run_tests: tests.out
	./tests.out

tests.out: tests.o greyscale.o
	$(CXX) $(CXXFLAGS) -o $@ $^

tests.o: tests.cpp greyscale.o
	$(CXX) $(CXXFLAGS) -c $<

mapraview.out: mapraview.o greyscale.o unit.o
	$(CXX) $(CXXFLAGS) -o $@ $^

mapraview.o: mapraview.cpp greyscale.o
	$(CXX) $(CXXFLAGS) -c $<

greyscale.o: greyscale.cpp greyscale.h
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f main.o main.out greyscale.o
