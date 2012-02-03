CXXFLAGS = -g -O3

test:	test.o
	$(CXX) $(CXXFLAGS) test.o -o test
	./test

# DO NOT DELETE

test.o: list_fun.hpp LinkedList.hpp Thunk.hpp nullstream.hpp
