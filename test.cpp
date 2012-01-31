#include <iostream>
#include "LinkedList.hpp"

template<typename T, typename P>
class Closure
{
private:
    typedef T(*FunPtr)(const P);

    const FunPtr code_;
    const P arg_;
public:
    Closure() :
        code_(),
        arg_()
    {
    }

    Closure(const FunPtr code, const P arg) :
        code_(code),
        arg_(arg)
    {
    }

    T operator()() const
    {
        return (*code_)(arg_);
    }
};

template<typename T, typename P>
Closure<T, P> closure(T(&code)(P), P arg)
{
    return Closure<T, P>(code, arg);
}


int makeInt(const int n)
{
    std::cout << "makeInt(" << n << ")" << std::endl;
    return n;
}


typedef odf::LinkedList<int, Closure<int, int> > IntList;

int main()
{
    IntList one(closure(makeInt, 1));
    IntList two(closure(makeInt, 2), &one);
    IntList three(3, &two);

    std::cout << three.value() << " "
              << three.next()->value() << " "
              << three.next()->next()->value() << std::endl;
    std::cout << three.value() << " "
              << three.next()->value() << " "
              << three.next()->next()->value() << std::endl;
}
