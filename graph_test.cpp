#include <iostream>
#include <utility>

#include "list_fun.hpp"
#include "Graph.hpp"

using namespace odf;

using std::cout;
using std::endl;

int main()
{
    typedef std::pair<int, int> E;

    E edges[] = { E(1, 2), E(1, 3), E(2, 4), E(3, 4), E(4, 5) };

    Graph<int> G(asList(edges));

    cout << "Graph has " << G.nvertices() << " vertices:" << endl;
    cout << G.vertices() << endl << endl;

    cout << "Graph has " << G.nedges() << " edges:" << endl;
    //cout << G.edges() << endl << endl;
}

/*
** Local Variables:
** mode:C++
** compile-command: "make -k graph_test"
** End:
*/
