#include <algorithm>
#include <iostream>
#include "LinkedList.hpp"

void printValue(int val)
{
    std::cout << " " << val;
}

typedef odf::LinkedList<int> IntList;
typedef IntList::Iterator Iter;

int main()
{
    IntList empty;
    IntList one(1);
    IntList two(2, &one);

    std::cout << "First:  " << *two << std::endl;
    std::cout << "Second: " << *two.next() << std::endl;
    std::cout << "First is last:  " << (two.next() == empty) << std::endl;
    std::cout << "Second is last: " << (two.next().next() == empty) << std::endl;
    std::cout << "First is not last:  " << (two.next() != empty) << std::endl;
    std::cout << "Second is not last: " << (two.next().next() != empty) << std::endl;

    std::for_each((Iter) two, (Iter) empty, printValue);
}
