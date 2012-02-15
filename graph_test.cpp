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

int main()
{
    typedef std::pair<int, int> E;

    E edges[] = { E(1, 2), E(1, 3), E(2, 4), E(3, 4), E(4, 5) };

    Graph<int> G(asList(edges));

    cout << endl;
    cout << "Graph has " << G.nrVertices() << " vertices:" << endl;
    cout << G.vertices() << endl;

    cout << endl;
    cout << "Graph has " << G.nrEdges() << " edges:" << endl;
    cout << G.edges() << endl;
}

/*
** Local Variables:
** mode:C++
** compile-command: "make -k graph_test"
** End:
*/
