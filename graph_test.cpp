#include <string>
#include <sstream>
#include <iostream>
#include <utility>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "list_fun.hpp"
#include "Graph.hpp"
#include "graph_serialization.hpp"

using namespace odf;

using std::cout;
using std::endl;
using std::pair;

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

template<typename G, typename F>
typename Graph<typename function_traits<F>::result_type>::edge_type
mapEdge(const F fun, const typename G::edge_type e)
{
    return typename Graph<typename function_traits<F>::result_type>
        ::edge_type(fun(e.first), fun(e.second));
}

template<typename G, typename F>
Graph<typename function_traits<F>::result_type>
mapGraph(const G& graph, const F fun)
{
    return Graph<typename function_traits<F>::result_type>(
        mapList(graph.edges(), curry(mapEdge<G,F>, fun)),
        mapList(graph.vertices(), fun));
}

std::string asString(int n)
{
    std::stringstream ss;
    ss << "-" << n << "-";
    return ss.str();
}

int main()
{
    typedef std::pair<int, int> E;

    E edges[] = { E(1, 2), E(1, 3), E(2, 4), E(3, 4), E(4, 5) };
    Graph<int> G(asList(edges));

    cout << endl << "Original graph:" << endl;
    printGraph(G);

    cout << endl << "Without vertex 2:" << endl;
    printGraph(Graph<int>(G).removeVertex(2));

    cout << endl << "Without (nonexistent) vertex 6:" << endl;
    printGraph(Graph<int>(G).removeVertex(6));

    cout << endl << "Without edge (4,5):" << endl;
    printGraph(Graph<int>(G).removeEdge(4, 5));

    cout << endl << "As before with vertices mapped to strings:" << endl;
    printGraph(mapGraph(Graph<int>(G).removeEdge(4, 5), asString));

    cout << endl << "Without (nonexistent) edge (3,5):" << endl;
    printGraph(Graph<int>(G).removeEdge(3, 5));

    std::stringstream ss;
    boost::archive::text_oarchive oa(ss);
    oa << G;
    cout << endl << ss.str() << endl;

    Graph<int> H;
    boost::archive::text_iarchive ia(ss);
    ia >> H;
    cout << endl << "Restored graph after serialization round trip:" << endl;
    printGraph(H);
}

/*
** Local Variables:
** mode:C++
** compile-command: "make -k graph_test"
** End:
*/
