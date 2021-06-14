CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -O3 -g

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

run_tests: bildtest results
	./bildtest -a1 results/result01.pgm
	./bildtest -a2 results/result02.pgm
	./bildtest -a3 results/result03.pgm
	./bildtest -a4 results/result04.pgm
	./bildtest -a5 results/result05.pgm
	./bildtest -a6 results/result06.pgm
	./bildtest -a7 results/result07.pgm
	./bildtest -a8 results/result08.pgm
	./bildtest -a9 results/result09.pgm
	./bildtest -a10 results/result10.pgm
	./bildtest -a11 results/result11.pgm

results: mapraview.out
	mkdir -p results
	./mapraview.out d r bilder/mapra.pgm w results/result01.pgm q
	./mapraview.out d r bilder/puppenbrunnen.pgm w results/result02.pgm q
	./mapraview.out d r bilder/dom.pgm m m m m m w results/result03.pgm q
	./mapraview.out d r bilder/dom.pgm B B w results/result04.pgm q
	./mapraview.out d r bilder/shuttle.pgm k C w results/result05.pgm q
	./mapraview.out d r bilder/shuttle.pgm l w results/result06.pgm q
	./mapraview.out d r bilder/shuttle.pgm s w results/result07.pgm q
	./mapraview.out d r bilder/bauteil.pgm C b w results/result08.pgm q
	./mapraview.out d r bilder/bauteil.pgm s i w results/result09.pgm q
	./mapraview.out d r bilder/roentgen.pgm S S S C w results/result10.pgm q
	./mapraview.out d r results/result04.pgm L L L L L w results/result11.pgm q

clean:
	rm -r -f mapraview.o mapraview.out greyscale.o results
