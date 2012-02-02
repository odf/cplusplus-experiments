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

    T first_;

public:
    List() :
        Ptr(),
        first_()
    {
        //TODO make the empty list recognisable
    }

    List(T first, Ptr thunk) :
        Ptr(thunk),
        first_(first)
    {
    }
    
    List(T first) :
        Ptr(),
        first_(first)
    {
    }

    const T first()
    {
        return first_;
    }
    
    bool atEnd()
    {
        return Ptr::isEmpty();
    }

    List rest()
    {
        if (atEnd())
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
    out << list.first();
    for (List<T> p = list; !p.atEnd(); p = p.rest())
    {
        out << " " << p.rest().first();
    }
    return out;
}

}

#endif // !ODF_LINKEDLIST_HPP
