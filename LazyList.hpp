#ifndef ODF_LAZYLIST_HPP
#define ODF_LAZYLIST_HPP

#include <iostream>
#include "LinkedList.hpp"

namespace odf
{

template<typename T>
class LazyList : public LinkedList<T>
{
protected:
    typedef LinkedList<T> List;
    typedef typename List::Ptr Ptr;
    typedef T(*FunPtr)();

    FunPtr code_;

public:
    LazyList(FunPtr code, const List *next) :
        List(next),
        code_(code)
    {
    }
    
    LazyList(FunPtr code) :
        List(),
        code_(code)
    {
    }

    const T *get()
    {
        if (code_ != 0) {
            Ptr::reset(new T((*code_)()));
            code_ = 0;
        }
        return Ptr::get();
    }

    const T operator*()
    {
        return *get();
    }
};

}

#endif // !ODF_LAZYLIST_HPP
