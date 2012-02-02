#ifndef ODF_LINKEDLIST_HPP
#define ODF_LINKEDLIST_HPP 1

#include "Thunk.hpp"

namespace odf
{

template<typename T>
class List : public Thunk<List<T> >
{
private:
    T first_;

public:
    List(T first, List<T> rest) :
        Thunk<List<T> >(rest),
        first_(first)
    {
    }
    
    List(T first) :
        Thunk<List<T> >(),
        first_(first)
    {
    }

    const T first()
    {
        return first_;
    }

    List rest()
    {
        return this->operator()();
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
