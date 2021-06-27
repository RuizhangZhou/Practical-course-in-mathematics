CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -O3 -g

.PHONY: clean

run: mapraview.out
	./mapraview.out

all: mapraview.out

mapraview.out: mapraview.o greyscale.o unit.o
	$(CXX) $(CXXFLAGS) -o $@ $^

mapraview.o: mapraview.cpp greyscale.o
	$(CXX) $(CXXFLAGS) -DSET_FORMAT -c $<

greyscale.o: greyscale.cpp greyscale.h
	$(CXX) $(CXXFLAGS) -c $<

run_tests: bildtest results
	./bildtest -a12 results/result12.pgm
	./bildtest -a13 results/result13.pgm
	./bildtest -a14 results/result14.pgm
	./bildtest -a15 results/result15.pgm
	./bildtest -a16 results/result16.pgm
	./bildtest -a17 results/result17.pgm
	# ./mapraview.out r results/result12.pgm q
	# ./mapraview.out r results/result13.pgm q
	# ./mapraview.out r results/result14.pgm q
	# ./mapraview.out r results/result15.pgm q
	# ./mapraview.out r results/result16.pgm q
	# ./mapraview.out r results/result17.pgm q

results: mapraview.out
	mkdir -p results
	./mapraview.out d f 1 r bilder/roentgen.pgm w results/result12.pgm q
	./mapraview.out d f 2 r bilder/roentgen.pgm w results/result13.pgm q
	./mapraview.out d f 3 r bilder/roentgen.pgm w results/result14.pgm q
	./mapraview.out d f 1 r bilder/shuttle.pgm w results/result15.pgm q
	./mapraview.out d f 2 r bilder/shuttle.pgm w results/result16.pgm q
	./mapraview.out d f 3 r bilder/shuttle.pgm w results/result17.pgm q

clean:
	rm -r -f mapraview.o mapraview.out greyscale.o results
