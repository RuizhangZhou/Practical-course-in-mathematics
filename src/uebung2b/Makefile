CXXFLAGS = -O0 -g -std=c++17 -Wall -Wextra -Wpedantic

run: main.out
	./$^

all: main.out

main.out: main.o unit.o student.o
	$(CXX) $(CXXFLAGS) -o $@ $^

main.o: main.cpp student.h
	$(CXX) $(CXXFLAGS) -c $<

student.o: student.cpp student.h
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f student.o main.out main.o

.PHONY: clean
