CXXFLAGS = -g -O3 -I$(HOME)/include

test:	test.o
	$(CXX) $(CXXFLAGS) test.o -o test -lgmp -lm
	./test

graph_test:	graph_test.o
	$(CXX) $(CXXFLAGS) graph_test.o -o graph_test -lgmp -lm
	./graph_test

json_spirit_test: json_spirit_test.o
	$(CXX) $(CXXFLAGS) json_spirit_test.o -o json_spirit_test -ljson_spirit

boost_mpi_test.o: boost_mpi_test.cpp
	mpic++ $(CXXFLAGS) -c boost_mpi_test.cpp -o boost_mpi_test.o

boost_mpi_test: boost_mpi_test.o
	mpic++ $(CXXFLAGS) boost_mpi_test.o -o boost_mpi_test \
	  -lboost_mpi-mt -lboost_serialization-mt

clean:
	rm -f *.o Makefile.bak

distclean:	clean
	rm -f test json_spirit_test boost_mpi_test bgl_tour graph_test


# DO NOT DELETE

test.o: Integer.h shared_array.hpp list.hpp thunk.hpp nullstream.hpp fun.hpp
test.o: list_fun.hpp
graph_test.o: graph.hpp list.hpp thunk.hpp nullstream.hpp list_fun.hpp
graph_test.o: fun.hpp
