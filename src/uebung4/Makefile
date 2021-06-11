CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -O3

.PHONY: clean

run: mapraview.out
	./mapraview.out

all: mapraview.out

mapraview.out: mapraview.o greyscale.o unit.o
	$(CXX) $(CXXFLAGS) -o $@ $^

mapraview.o: mapraview.cpp greyscale.o
	$(CXX) $(CXXFLAGS) -c $<

greyscale.o: greyscale.cpp greyscale.h
	$(CXX) $(CXXFLAGS) -c $<

examples: bildtest results
	./bildtest -a1 results/result01.pgm

results: mapraview.out
	mkdir -p results
	./mapraview.out "r bilder/mapra.pgm w results/result01.pgm q"
	./mapraview.out "r bilder/puppenbrunnen.pgm w results/result02.pgm q"
	./mapraview.out r bilder/barcode.pgm
	./mapraview.out -r bilder/bauteil.pgm
	./mapraview.out -r bilder/bricks.pgm
	./mapraview.out -r bilder/dom.pgm
	./mapraview.out -r bilder/leaf.pgm
	./mapraview.out -r bilder/roentgen.pgm
	./mapraview.out -r bilder/rose.pgm
	./mapraview.out -r bilder/shuttle.pgm
	./mapraview.out -r bilder/weave.pgm

clean:
	rm -f -r main.o mapraview.out greyscale.o results tests.o tests.out
