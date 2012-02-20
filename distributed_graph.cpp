#include <iostream>
#include <sstream>

#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>
#include <boost/mpi.hpp>

#include "List.hpp"
#include "fun.hpp"
#include "list_fun.hpp"
#include "Graph.hpp"
#include "graph_serialization.hpp"

namespace mpi = boost::mpi;
using namespace odf;

using std::cout;
using std::endl;
using std::pair;
using std::ostream;
using std::string;
using std::stringstream;
using boost::unordered_set;
using boost::unordered_map;

namespace odf {

template<class G>
struct GraphDelegate
{
    GraphDelegate(const G graph) :
        nrVertices(&G::nrVertices, graph),
        vertices(&G::vertices, graph),
        incidences(&G::incidences, graph)
    {
    }

    typename G::vertex_type typedef vertex_type;
    typename G::edge_type   typedef edge_type;

private:
    typedef int (G::*nrVerticesFunc)() const;
    typedef List<vertex_type> (G::*verticesFunc)() const;
    typedef List<edge_type> (G::*incidencesFunc)(const vertex_type&) const;

public:
    typename currierTraits<nrVerticesFunc>::result_type nrVertices;
    typename currierTraits<verticesFunc>::result_type   vertices;
    typename currierTraits<incidencesFunc>::result_type incidences;
};

template<class G>
inline GraphDelegate<G> delegate(const G graph)
{
    return GraphDelegate<G>(graph);
}

template<typename T>
std::ostream& operator<<(std::ostream& out, const pair<T, T>& edge)
{
    out << "(" << edge.first << "," << edge.second << ")";
}

}

template<typename T>
ostream& operator<<(ostream& out, const Graph<T>& G)
{
    out << "  " << G.nrVertices() << " vertices: " << G.vertices() << endl;
    out << "  " << G.nrEdges()    << " edges:    " << G.edges() << endl;
    return out;
}


List<int>
sizeSequence(int total, int parts, int carry)
{
    return makeList((carry + total) / parts,
                    curry(sizeSequence, total, parts, (carry + total) % parts));
}

List<int>
sizesForPartition(int total, int parts)
{
    return takeList(sizeSequence(total, parts, 0), parts);
}

template<typename T>
List<List<T> > partitionList(const List<T> source, const List<int> sizes)
{
    if (source.isEmpty())
    {
        return List<List<T> >();
    }
    else if (sizes.isEmpty())
    {
        return makeList(source);
    }
    else
    {
        const int k = sizes.first();
        return makeList(takeList(source, k),
                        curry(partitionList<T>,
                              dropList(source, k), sizes.rest()));
    }
}

template<typename T>
List<List<T> > partitionList(const List<T> source, int total, int parts)
{
    return partitionList(source, sizesForPartition(total, parts));
}

template<typename G>
Graph<typename G::vertex_type> extendedSubgraph(
    const G graph, const List<typename G::vertex_type> vertices)
{
    return Graph<typename G::vertex_type>(
        flatMap(vertices, graph.incidences), vertices);
}

template<typename G>
List<Graph<typename G::vertex_type> > graphPartition(
    const G graph, const int parts)
{
    return mapList(partitionList(graph.vertices(), graph.nrVertices(), parts),
                   curry(extendedSubgraph<G>, graph));
}

template<typename T>
unordered_map<T, int> numberingForPartition(const List<List<T> > parts)
{
    unordered_map<T, int> result;
    int i = 0;

    for (List<List<T> > p = parts; not p.isEmpty(); p = p.rest())
    {
        for (List<T> q = p.first(); not q.isEmpty(); q = q.rest())
        {
            result[q.first()] = i;
        }
        ++i;
    }

    return result;
}

Graph<int> makeGraph()
{
    typedef Graph<int>::edge_type E;
    E edges[] = { E(1, 2), E(1, 3), E(2, 4), E(3, 4), E(4, 5) };

    return Graph<int>(asList(edges));
}


int main(int argc, char* argv[])
{
    int rank, k;
    unordered_map<int, int> assignment;
    Graph<int> H;

    mpi::environment env(argc, argv);
    mpi::communicator world;

    rank = world.rank();
    k = world.size();

    if (rank == 0)
    {
        int i = 0;

        for (List<Graph<int> > p = graphPartition(delegate(makeGraph()), k-1);
             not p.isEmpty();
             p = p.rest())
        {
            world.send(++i, 0, p.first());
        }

        string result;
        for (i = 1; i < k; ++i)
        {
            world.recv(i, 1, result);
            cout << result;
        }
    }
    else
    {
        world.recv(0, 0, H);
        stringstream ss;
        ss << "Graph received by process " << rank << ":" << endl << H << endl;
        world.send(0, 1, ss.str());
    }
}

/*
** Local Variables:
** mode:C++
** compile-command: "make -k distributed_graph"
** End:
*/
