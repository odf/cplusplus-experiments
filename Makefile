CXXFLAGS = -g -O3 -I$(HOME)/include

test:	test.o
	$(CXX) $(CXXFLAGS) test.o -o test -lgmp -lm
	./test

json_spirit_test: json_spirit_test.o
	$(CXX) $(CXXFLAGS) json_spirit_test.o -o json_spirit_test -ljson_spirit

mpi_test.o: mpi_test.cpp
	mpic++ $(CXXFLAGS) -c mpi_test.cpp -o mpi_test.o

mpi_test: mpi_test.o
	mpic++ $(CXXFLAGS) mpi_test.o -o mpi_test

clean:
	rm -f *.o

distclean:	clean
	rm -f test json_spirit_test mpi_test bgl_tour


# DO NOT DELETE

test.o: Integer.h shared_array.hpp LinkedList.hpp Thunk.hpp nullstream.hpp
test.o: fun.hpp list_fun.hpp
