CXXFLAGS = -g -O3 -std=c++0x

test:	test.o
	$(CXX) $(CXXFLAGS) test.o -o test
	./test

# DO NOT DELETE

test.o: LinkedList.hpp
