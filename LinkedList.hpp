#ifndef ODF_LINKEDLIST_HPP
#define ODF_LINKEDLIST_HPP 1

#include <iostream>
#include "Thunk.hpp"

namespace odf
{

template<typename T>
class List : Thunk<List<T> >
{
private:
    typedef Thunk<List<T> > Ptr;

    bool is_empty_;
    T first_;

public:
    List() :
        Ptr(),
        is_empty_(true),
        first_()
    {
    }

    List(T first, Ptr thunk) :
        Ptr(thunk),
        is_empty_(false),
        first_(first)
    {
    }
    
    List(T first) :
        Ptr(),
        is_empty_(false),
        first_(first)
    {
    }

    const T first()
    {
        return first_;
    }
    
    bool isEmpty()
    {
        return is_empty_;
    }

    List rest()
    {
        if (Ptr::isEmpty())
        {
            return List();
        }
        else
        {
            return Ptr::operator()();
        }
    }
};

template<typename T>
List<T> cons(const T first)
{
    return List<T>(first);
}

template<typename T>
List<T> cons(const T first, const List<T> rest)
{
    return List<T>(first, rest);
}

template<typename T, typename Functor>
List<T> cons(const T first, const Functor code)
{
    return List<T>(first, makeThunk<List<T> >(code));
}

template<typename T>
std::ostream& operator<<(std::ostream& out, List<T> list)
{
    if (!list.isEmpty()) {
        out << list.first();
        for (List<T> p = list.rest(); !p.isEmpty(); p = p.rest())
        {
            out << " " << p.first();
        }
    }
    return out;
}

}

#endif // !ODF_LINKEDLIST_HPP
