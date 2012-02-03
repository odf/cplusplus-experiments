#ifndef ODF_LIST_FUN_HPP
#define ODF_LIST_FUN_HPP 1

#include <iostream>
#include "LinkedList.hpp"

using namespace boost::lambda;

namespace odf
{

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

template<typename T, typename Functor>
void forEach(const List<T> list, const Functor f)
{
    for (List<T> p = list; !p.isEmpty(); p = p.rest())
    {
        f(p.first());
    }
}

template<typename T, typename Functor>
List<T> mapList(const List<T> source, const Functor f);

template<typename T, typename Functor>
struct mapRest
{
    mapRest(const List<T> source, const Functor f) :
        source_(source),
        f_(f)
    {
    }

    const List<T> operator() () const
    {
        return mapList(source_.rest(), f_);
    }
private:
    const List<T> source_;
    const Functor f_;
};

template<typename T, typename Functor>
List<T> mapList(const List<T> source, const Functor f)
{
    if (source.isEmpty())
    {
        return source;
    }
    else
    {
        return cons(f(source.first()), mapRest<T, Functor>(source, f));
    }
}

template<typename T, typename Functor>
List<T> filterList(const List<T> source, const Functor predicate);

template<typename T, typename Functor>
struct filterRest
{
    filterRest(const List<T> source, const Functor f) :
        source_(source),
        f_(f)
    {
    }

    const List<T> operator() () const
    {
        return filterList(source_.rest(), f_);
    }
private:
    const List<T> source_;
    const Functor f_;
};

template<typename T, typename Functor>
List<T> filterList(const List<T> source, const Functor predicate)
{
    List<T> p = source;
    while (not (p.isEmpty() or predicate(p.first())))
    {
        p = p.rest();
    }

    if (p.isEmpty())
    {
        return p;
    }
    else
    {
        return cons(p.first(), filterRest<T, Functor>(p, predicate));
    }
}

template<typename T>
std::ostream& operator<<(std::ostream& out, const List<T> list)
{
    if (!list.isEmpty())
    {
        out << list.first();
        for (List<T> p = list.rest(); !p.isEmpty(); p = p.rest())
        {
            out << " " << p.first();
        }
    }
    return out;
}

}

#endif // !ODF_LIST_FUN_HPP
