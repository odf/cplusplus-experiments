#include <iostream>
#include "LinkedList.hpp"

int main()
{
    odf::LinkedList<int> one(1);
    odf::LinkedList<int> two(2, &one);

    std::cout << "First:  " << *two << std::endl;
    std::cout << "Second: " << *two.next() << std::endl;
    std::cout << "First is last:  " << (two.next() == 0) << std::endl;
    std::cout << "Second is last: " << (two.next().next() == 0) << std::endl;
}
