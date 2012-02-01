#include <iostream>
#include "LinkedList.hpp"

using namespace odf;

template<typename T>
struct constant_functor
{
    T val_;

    constant_functor(T val)
    {
        val_ = val;
    }

    T operator() ()
    {
        std::cout << "evaluating constant " << val_ << std::endl;
        return val_;
    }
};

template<typename T>
constant_functor<T> constant(const T val)
{
    return constant_functor<T>(val);
}


template<typename F>
void print_twice(F f, std::string t)
{
    std::cout << t << f() << std::endl;
    std::cout << t << f() << std::endl;
}


int main()
{
    List<int> one   = makeList<int>(constant(1));
    List<int> two   = makeList<int>(constant(2), one);
    List<int> three = makeList<int>(constant(3), two);

    std::cout << three->first() << " "
              << three->rest()->first() << " "
              << three->rest()->rest()->first() << std::endl;
    std::cout << three->first() << " "
              << three->rest()->first() << " "
              << three->rest()->rest()->first() << std::endl;

    print_twice(makeThunk<int>(constant(4)), "thunk() = ");
}
