CXXFLAGS = -g -O3

test:	test.o
	$(CXX) $(CXXFLAGS) test.o -o test -lgmp -lm
	./test

# DO NOT DELETE

test.o: list_fun.hpp LinkedList.hpp Thunk.hpp nullstream.hpp
