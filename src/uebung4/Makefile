CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -O3

.PHONY: clean

run: main.out
	./main.out

all: main.out

main.out: mapraview.o greyscale.o unit.o
	$(CXX) $(CXXFLAGS) -o $@ $^

mapraview.o: mapraview.cpp greyscale.o
	$(CXX) $(CXXFLAGS) -c $<

greyscale.o: greyscale.cpp greyscale.h
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f main.o main.out greyscale.o