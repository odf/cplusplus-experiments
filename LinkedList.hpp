#ifndef ODF_LINKEDLIST_HPP
#define ODF_LINKEDLIST_HPP 1

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

#endif // !ODF_LINKEDLIST_HPP
