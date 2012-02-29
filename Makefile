CXXWARNS = -Wall -Wextra -pedantic
CXXFLAGS = $(CXXWARNS) -g -O3
PROGRAMS = list_test hash_trie_test graph_test boost_mpi_test distributed_graph

UNITTEST = ../unittest-cpp/UnitTest++

list_test:		list_test.o
	$(CXX) $(CXXFLAGS) list_test.o -o list_test -lgmp -lm

graph_test:		graph_test.o
	$(CXX) $(CXXFLAGS) graph_test.o -o graph_test -lboost_serialization-mt

hash_trie_test.o:	hash_trie_test.cpp
	$(CXX) $(CXXFLAGS) -I$(UNITTEST)/src -c \
	     hash_trie_test.cpp -o hash_trie_test.o

hash_trie_test:		hash_trie_test.o
	$(CXX) $(CXXFLAGS) hash_trie_test.o -o hash_trie_test \
	    -L$(UNITTEST) -lUnitTest++

boost_mpi_test.o: 	boost_mpi_test.cpp
	mpic++ $(CXXFLAGS) -c boost_mpi_test.cpp -o boost_mpi_test.o

boost_mpi_test: 	boost_mpi_test.o
	mpic++ $(CXXFLAGS) boost_mpi_test.o -o boost_mpi_test \
	    -lboost_mpi-mt -lboost_serialization-mt

distributed_graph.o:	distributed_graph.cpp
	mpic++ $(CXXFLAGS) -c distributed_graph.cpp -o distributed_graph.o

distributed_graph:	distributed_graph.o
	mpic++ $(CXXFLAGS) distributed_graph.o -o distributed_graph \
	    -lboost_mpi-mt -lboost_serialization-mt

json_spirit_test: json_spirit_test.o
	$(CXX) $(CXXFLAGS) json_spirit_test.o -o json_spirit_test -ljson_spirit

all:	$(PROGRAMS)

clean:
	rm -f *.o Makefile.bak

distclean:	clean
	rm -f $(PROGRAMS)

depend:
	makedepend -Y. list_test.cpp hash_trie_test.cpp graph_test.cpp \
	distributed_graph.cpp

# DO NOT DELETE

list_test.o: Integer.h shared_array.hpp List.hpp Thunk.hpp nullstream.hpp
list_test.o: fun.hpp list_fun.hpp
hash_trie_test.o: hash_trie.hpp
graph_test.o: list_fun.hpp List.hpp Thunk.hpp nullstream.hpp fun.hpp
graph_test.o: Graph.hpp graph_serialization.hpp
distributed_graph.o: List.hpp Thunk.hpp nullstream.hpp fun.hpp list_fun.hpp
distributed_graph.o: Graph.hpp graph_serialization.hpp
