#include <algorithm>
#include <iostream>
#include "LinkedList.hpp"

void printValue(int val)
{
    std::cout << " " << val;
}

typedef odf::LinkedList<int> IntList;

int main()
{
    IntList one(1);
    IntList two(2, &one);

    std::for_each(two.begin(), two.end(), printValue);
}
