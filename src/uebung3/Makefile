CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -O2

.PHONY: clean test_mv run_test test run_test

run: main.out
	./main.out

all: main.out

run_test_mv: test_mv.out
	./test_mv.out

test_mv: test_mv.out

run_test: test.out
	./test.out

test: test.out

test.out: test.o vector.o sparse_matrix.o
	$(CXX) $(CXXFLAGS) -o $@ $^

test.o: test.cpp vector.o sparse_matrix.o
	$(CXX) $(CXXFLAGS) -c $<

main.out: main.o vector.o sparse_matrix.o unit.o
	$(CXX) $(CXXFLAGS) -o $@ $^

main.o: main.cpp vector.o sparse_matrix.o
	$(CXX) $(CXXFLAGS) -c $<

test_mv.out: vector.o sparse_matrix.o test_mv.o
	$(CXX) $(CXXFLAGS) -o $@ vector.o sparse_matrix.o test_mv.o

sparse_matrix.o: sparse_matrix.cpp sparse_matrix.h vector.h
	$(CXX) $(CXXFLAGS) -c $<

vector.o: vector.cpp vector.h
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f main.o vector.o sparse_matrix.o main.out test_mv.out
