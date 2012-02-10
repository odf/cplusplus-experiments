CXXFLAGS = -g -O3

test:	test.o
	$(CXX) $(CXXFLAGS) test.o -o test -lgmp -lm
	./test

json_spirit_test: json_spirit_test.o
	$(CXX) $(CXXFLAGS) json_spirit_test.o -o json_spirit_test -ljson_spirit

# DO NOT DELETE

test.o: Integer.h shared_array.hpp LinkedList.hpp Thunk.hpp nullstream.hpp
test.o: fun.hpp list_fun.hpp
