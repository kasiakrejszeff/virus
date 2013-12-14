CXX=g++
CXXFLAGS= -Wall -Wextra -std=c++11 -g

all: test

test: virus_genealogy.h test.cc
	$(CXX) $(CXXFLAGS) test.cc -o test
	
clean:
	rm -f *.o
	rm -f test

.PHONY: clean all