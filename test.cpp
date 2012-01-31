#include <iostream>
#include "LinkedList.hpp"

using namespace odf;

int makeInt(const int n)
{
    std::cout << "makeInt(" << n << ")" << std::endl;
    return n;
}

int make_1()
{
    return makeInt(1);
}

int make_2()
{
    return makeInt(2);
}

int make_3()
{
    return makeInt(3);
}

template<typename F>
void print_twice(F f, std::string t)
{
    std::cout << t << f() << std::endl;
    std::cout << t << f() << std::endl;
}


typedef AbstractList<int>::Ptr List;

int main()
{
    List one = &makeList<int>(make_1);
    List two = &makeList<int>(make_2, one);
    List three = &makeList<int>(make_3, two);

    std::cout << three->value() << " "
              << three->next()->value() << " "
              << three->next()->next()->value() << std::endl;
    std::cout << three->value() << " "
              << three->next()->value() << " "
              << three->next()->next()->value() << std::endl;

    print_twice(makeThunk<int>(make_3), "thunk() = ");
}
