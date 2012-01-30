#include <iostream>
#include "LinkedList.hpp"

void printValue(int val)
{
    std::cout << "--" << val << "--" << std::endl;
}

int _1()
{
    std::cout << "Computing the value 1" << std::endl;
    return 1;
}

int _2()
{
    std::cout << "Computing the value 2" << std::endl;
    return 2;
}

typedef odf::LinkedList<int> IntList;

int main()
{
    IntList one(_1);
    IntList two(_2, &one);
    IntList three(3, &two);

    std::cout << three.value() << " "
              << three.next()->value() << " "
              << three.next()->next()->value() << std::endl;
    std::cout << three.value() << " "
              << three.next()->value() << " "
              << three.next()->next()->value() << std::endl;
}
