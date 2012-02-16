#include <iostream>

#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>
#include <boost/mpi.hpp>

#include "List.hpp"
#include "fun.hpp"
#include "list_fun.hpp"
#include "Graph.hpp"

namespace mpi = boost::mpi;
using namespace odf;

using std::cout;
using std::endl;
using std::pair;
using boost::unordered_set;
using boost::unordered_map;


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

template<typename T>
pair<T, T> makePair(const T& a, const T& b)
{
    return pair<T, T>(a, b);
}

template<typename T, typename F>
pair<typename function_traits<F>::result_type,
     typename function_traits<F>::result_type>
mapPair(const F fun, const pair<T, T>& p)
{
    return makePair(fun(p.first), fun(p.second));
}

template<typename T, typename F>
Graph<typename function_traits<F>::result_type>
mapGraph(const Graph<T>& graph, const F fun)
{
    return Graph<typename function_traits<F>::result_type>(
        mapList(graph.edges(), curry(mapPair<T, F>, fun)));
}

Graph<int> makeGraph()
{
    typedef Graph<int>::edge_type E;
    E edges[] = { E(1, 2), E(1, 3), E(2, 4), E(3, 4), E(4, 5) };

    return Graph<int>(asList(edges));
}


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
