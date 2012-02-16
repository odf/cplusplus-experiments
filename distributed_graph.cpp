#include <boost/mpi.hpp>
#include <iostream>

#include "List.hpp"
#include "list_fun.hpp"
#include "Graph.hpp"

namespace mpi = boost::mpi;
using namespace odf;

List<int>
sizeSequence(int total, int parts, int carry)
{
    return makeList((carry + total) / parts,
                    curry(sizeSequence, total, parts, (carry + total) % parts));
}

List<int>
splitSizes(int total, int parts)
{
    return takeList(sizeSequence(total, parts, 0), parts);
}

Graph<int> makeGraph()
{
    typedef Graph<int>::edge_type E;
    E edges[] = { E(1, 2), E(1, 3), E(2, 4), E(3, 4), E(4, 5) };

    return Graph<int>(asList(edges));
}

// template<typename T>
// List<Graph<T> > splitGraph(const Graph<T>& graph, int n)
// {
//     int N = graph.nrVertices();

//     return splitGraph(graph, graph.vertices(), n, N, 0);
// }

using std::cout;
using std::endl;


int main(int argc, char* argv[])
{
    int rank;

    mpi::environment env(argc, argv);
    mpi::communicator world;

    rank = world.rank();

    if (rank == 0)
    {
        int total;
        List<int> sizes;

        std::cout << "Enter a size: " << std::endl;
        std::cin >> total;

        sizes = splitSizes(total, world.size());
        cout << "Sizes: " << sizes << endl;
        cout << "Total: " << sum(sizes) << endl;

        //List<Graph<int> > p = splitGraph(makeGraph(world.size()));

        //for (int i = 0; not p.isEmpty(); p = p.rest())
        //{
        //}
    }
}

/*
** Local Variables:
** mode:C++
** compile-command: "make -k distributed_graph"
** End:
*/
