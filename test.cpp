#include <iostream>
#include "LinkedList.hpp"

int makeInt(const int n)
{
    std::cout << "makeInt(" << n << ")" << std::endl;
    return n;
}


typedef odf::LinkedList<int> IntList;

int main()
{
    IntList one([&] () { return makeInt(1); });
    IntList two([&] () { return makeInt(2); }, &one);
    IntList three(3, &two);

    std::cout << three.value() << " "
              << three.next()->value() << " "
              << three.next()->next()->value() << std::endl;
    std::cout << three.value() << " "
              << three.next()->value() << " "
              << three.next()->next()->value() << std::endl;
}
