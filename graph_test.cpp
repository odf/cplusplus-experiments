#include <iostream>
#include <utility>

#include "list_fun.hpp"
#include "Graph.hpp"

using namespace odf;

int main()
{
    typedef std::pair<int, int> E;

    E edges[] = { E(1, 2), E(1, 3), E(2, 4), E(3, 4), E(4, 5) };

    Graph<int> G(asList(edges));

    std::cout << G.vertices() << std::endl;
}

/*
** Local Variables:
** mode:C++
** compile-command: "make -k graph_test"
** End:
*/
