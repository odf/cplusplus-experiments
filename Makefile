CXXFLAGS = -g -O3

test:	test.o
	$(CXX) $(CXXFLAGS) test.o -o test
	./test

# DO NOT DELETE

test.o: LinkedList.hpp Thunk.hpp
