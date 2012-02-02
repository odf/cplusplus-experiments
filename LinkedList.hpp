#ifndef ODF_LINKEDLIST_HPP
#define ODF_LINKEDLIST_HPP 1

#include "Thunk.hpp"

namespace odf
{

template<typename T>
class List : public Thunk<List<T> >
{
private:
    typedef Thunk<List<T> > Ptr;

    T first_;

public:
    List(T first, List<T> rest) :
        Ptr(rest),
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
        }
        else
        {
            return this->operator()();
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

}

#endif // !ODF_LINKEDLIST_HPP
