#include <iostream>
#include "LinkedList.hpp"

using namespace odf;

int main()
{
    List<int> one   = cons<int>(1);
    List<int> two   = cons<int>(2, one);
    List<int> three = cons<int>(3, two);
    List<int> four  = cons<int>(4, two);

    std::cout << three().first() << " "
              << three().rest().first() << " "
              << three().rest().rest().first() << std::endl;
    std::cout << four().first() << " "
              << four().rest().first() << " "
              << four().rest().rest().first() << std::endl;
}
