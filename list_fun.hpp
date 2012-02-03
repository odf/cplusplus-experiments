#ifndef ODF_LIST_FUN_HPP
#define ODF_LIST_FUN_HPP 1

#include <iostream>
#include "LinkedList.hpp"

using namespace boost::lambda;

namespace odf
{

template<typename T>
inline List<T> cons(const T first)
{
    return List<T>(first);
}

template<typename T>
inline List<T> cons(const T first, const List<T> rest)
{
    return List<T>(first, rest);
}

template<typename T, typename Functor>
inline List<T> cons(const T first, const Functor code)
{
    return List<T>(first, makeThunk<List<T> >(code));
}

template<typename T, typename Functor>
inline void forEach(const List<T> list, const Functor f)
{
    for (List<T> p = list; !p.isEmpty(); p = p.rest())
    {
        f(p.first());
    }
}

template<typename F, typename L, typename A>
struct binder
{
    binder(const F fun, const L src, const A arg) :
        fun(fun), src(src), arg(arg)
    {
    }

    const L operator() () const
    {
        return fun(src.rest(), arg);
    }

private:
    const F fun;
    const L src;
    const A arg;
};

template<typename F, typename L, typename A>
inline struct binder<F, L, A> bindRest(const F fun, const L src, const A arg)
{
    return binder<F, L, A>(fun, src, arg);
}

template<typename T, typename F>
List<T> mapList(const List<T> src, const F fun)
{
    if (src.isEmpty())
    {
        return src;
    }
    else
    {
        return cons(fun(src.first()), bindRest(mapList<T, F>, src, fun));
    }
}

template<typename T, typename F>
List<T> filterList(const List<T> src, const F pred)
{
    List<T> p = src;
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
        return cons(p.first(), bindRest(filterList<T, F>, p, pred));
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
