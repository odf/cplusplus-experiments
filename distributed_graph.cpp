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


namespace odf {

template<typename T>
std::ostream& operator<<(std::ostream& out, const pair<T, T>& edge)
{
    out << "(" << edge.first << "," << edge.second << ")";
}

}

template<typename T>
void print(const T val)
{
    cout << "<" << val << ">";
}

template<typename T>
void printEach(const List<T> list)
{
    forEach(list, print<T>);
    cout << endl;
}

template<typename T>
void printGraph(const Graph<T> G)
{
    cout << "  " << G.nrVertices() << " vertices: " << G.vertices() << endl;
    cout << "  " << G.nrEdges()    << " edges:    " << G.edges() << endl;
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
List<List<T> > partitionList(const List<T>& source, int total, int parts)
{
    return partitionList(source, sizesForPartition(total, parts));
}

template<typename T>
unordered_map<T, int> numberingForPartition(const List<List<T> >& parts)
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

template<typename T>
Graph<T> extendedSubgraph(const Graph<T>& graph, const List<T>& vertices)
{
    Graph<T> result;
    List<T> p, q;

    for (p = vertices; not p.isEmpty(); p = p.rest())
    {
        const T v = p.first();
        if (graph.hasVertex(v))
        {
            result.addVertex(v);
            for (q = graph.successors(v); not q.isEmpty(); q = q.rest())
            {
                result.addEdge(v, q.first());
            }
            for (q = graph.predecessors(v); not q.isEmpty(); q = q.rest())
            {
                result.addEdge(q.first(), v);
            }
        }
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
    int rank, k, N;
    unordered_map<int, int> assignment;
    Graph<int> G;

    mpi::environment env(argc, argv);
    mpi::communicator world;

    rank = world.rank();
    k = world.size();

    if (rank == 0)
    {
        G = makeGraph();
        N = G.nrVertices();
        List<List<int> > partition = partitionList(G.vertices(), N, k);
        assignment = numberingForPartition(partition);

        List<List<int> > p = partition;
        for (int i = 0; i < k; ++i)
        {
            List<int> verts = p.first();

            cout << "Subgraph " << i << ":" << endl;
            cout << "  (owned vertices: " << verts << ")" << endl;
            printGraph(extendedSubgraph(G, verts));
            cout << endl;
            p = p.rest();
        }
    }
}

/*
** Local Variables:
** mode:C++
** compile-command: "make -k distributed_graph"
** End:
*/
