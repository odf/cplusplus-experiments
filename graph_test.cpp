#include <string>
#include <sstream>
#include <iostream>
#include <utility>

#include "list_fun.hpp"
#include "Graph.hpp"

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

    cout << endl << "Without (nonexistent) edge (3,5):" << endl;
    printGraph(Graph<int>(G).removeEdge(3, 5));

    cout << endl << "With vertices mapped to strings:" << endl;
    printGraph(mapGraph(G, asString));
}

/*
** Local Variables:
** mode:C++
** compile-command: "make -k graph_test"
** End:
*/
