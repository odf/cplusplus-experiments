#ifndef ODF_LINKEDLIST_HPP
#define ODF_LINKEDLIST_HPP 1

#include "Thunk.hpp"

namespace odf
{

template<typename T>
class ListLink;

template<typename T>
class List : public Thunk<ListLink<T> >
{
private:
    typedef ListLink<T> Link;

public:
    List() :
        Thunk<Link>()
    {
    }

    List(Link link) :
        Thunk<Link>(link)
    {
    }

    const T first()
    {
        return (*this)().first();
    }

    List<T> rest()
    {
        return (*this)().rest();
    }
};

template<typename T>
class ListLink
{
protected:
    T first_;
    List<T> rest_;
    List<T> self_;

public:
    ListLink(T first, List<T> rest) :
        first_(first),
        rest_(rest)
    {
        self_ = List<T>(*this);
    }
    
    ListLink(T first) :
        first_(first),
        rest_()
    {
        self_ = List<T>(*this);
    }

    const T first()
    {
        return first_;
    }

    List<T> rest()
    {
        return rest_;
    }

    List<T> operator&()
    {
        return self_;
    }
};

template<typename T>
List<T> cons(const T first)
{
    return &ListLink<T>(first);
}

template<typename T>
List<T> cons(const T first, const List<T> rest)
{
    return &ListLink<T>(first, rest);
}

}

#endif // !ODF_LINKEDLIST_HPP
