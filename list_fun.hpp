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

template<typename Functor, typename ListT, typename ArgT>
struct bindRest
{
    bindRest(const Functor fun, const ListT src, const ArgT arg) :
        fun(fun), src(src), arg(arg)
    {
    }

    const ListT operator() () const
    {
        return fun(src.rest(), arg);
    }

private:
    const Functor fun;
    const ListT   src;
    const ArgT    arg;
};


template<typename T, typename Functor>
List<T> mapList(const List<T> source, const Functor f)
{
    typedef List<T> (*FunPtr)(const List<T>, const Functor);
    typedef bindRest<FunPtr, List<T>, Functor> binder;

    if (source.isEmpty())
    {
        return source;
    }
    else
    {
        return cons(f(source.first()), binder(mapList, source, f));
    }
}

template<typename T, typename Functor>
List<T> filterList(const List<T> source, const Functor pred)
{
    typedef List<T> (*FunPtr)(const List<T>, const Functor);
    typedef bindRest<FunPtr, List<T>, Functor> binder;

    List<T> p = source;
    while (not (p.isEmpty() or pred(p.first())))
    {
        p = p.rest();
    }

    if (p.isEmpty())
    {
        return p;
    }
    else
    {
        return cons(p.first(), binder(filterList, p, pred));
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
