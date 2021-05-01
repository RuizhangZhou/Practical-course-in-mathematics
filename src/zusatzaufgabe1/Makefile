CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -Wpedantic -fno-inline -fPIC -fPIE
LIBS  = -lGL -lpthread -lX11
# Mac 
# CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -Wpedantic -fno-inline -I/opt/X11/include
# LIBS  = -L/opt/X11/lib -lGLU -lGL -lX11

all: loesung

loesung.o: loesung.cpp unit.h
	$(CXX) $(CXXFLAGS) -c $<

loesung: loesung.o unit.o IGL.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)


clean:
	rm -f loesung loesung.o

.PHONY: clean
