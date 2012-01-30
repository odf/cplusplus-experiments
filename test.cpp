#include <algorithm>
#include <iostream>
#include "LinkedList.hpp"
#include "LazyList.hpp"

void printValue(int val)
{
    std::cout << " " << val;
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

//typedef odf::LinkedList<int> IntList;
typedef odf::LazyList<int> IntList;

int main()
{
    IntList one(_1);
    IntList two(_2, &one);

    std::cout << "*two = " << *two << std::endl;
    std::cout << "*two = " << *two << std::endl;
    std::cout << "*one = " << *one << std::endl;
    std::cout << "*one = " << *one << std::endl;

    std::for_each(two.begin(), two.end(), printValue);
}
