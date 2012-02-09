CXXFLAGS = -g -O3

test:	test.o
	$(CXX) $(CXXFLAGS) test.o -o test -lgmp -lm
	./test

# DO NOT DELETE

test.o: Integer.h shared_array.hpp LinkedList.hpp Thunk.hpp nullstream.hpp
test.o: ListIterator.hpp list_fun.hpp fun.hpp
