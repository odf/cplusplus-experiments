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

int main()
{
    List<int> three = cons(3, cons(2, cons(1)));
    List<int> four  = cons(4, three.rest());

    std::cout << three << std::endl;
    std::cout << four << std::endl;
}
