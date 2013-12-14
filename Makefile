CXX=g++
CXXFLAGS= -Wall -Wextra -std=c++11 -g
OBJECTS=test

all: $(OBJECTS)

test: virus_genealogy.h virus.h test.cc
	$(CXX) $(CXXFLAGS) test.cc -o test
	
clean:
	rm -f $(OBJECTS) *.o

.PHONY: clean all