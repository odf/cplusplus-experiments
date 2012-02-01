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


typedef AbstractList<int>::Ptr List;

int main()
{
    List one = &makeList<int>(constant(1));
    List two = &makeList<int>(constant(2), one);
    List three = &makeList<int>(constant(3), two);

    std::cout << three->value() << " "
              << three->next()->value() << " "
              << three->next()->next()->value() << std::endl;
    std::cout << three->value() << " "
              << three->next()->value() << " "
              << three->next()->next()->value() << std::endl;

    print_twice(makeThunk<int>(constant(4)), "thunk() = ");
}
