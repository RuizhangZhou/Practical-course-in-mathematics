CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -O2

.PHONY: clean

A3: #Hier fehlt was
	#Hier fehlt was

meine_loesung.o: #Hier fehlt was
	#Hier fehlt was

test_mv: vector.o sparse_matrix.o test_mv.o
	$(CXX) $(CXXFLAGS) -o $@ vector.o sparse_matrix.o test_mv.o
	
sparse_matrix.o: sparse_matrix.cpp sparse_matrix.h vector.h
	$(CXX) $(CXXFLAGS) -c $<

vector.o: vector.cpp vector.h
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f A3 meine_loesung.o vector.o sparse_matrix.o

