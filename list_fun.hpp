#ifndef ODF_LIST_FUN_HPP
#define ODF_LIST_FUN_HPP 1

#include <functional>
#include "fun.hpp"

namespace odf
{

template<typename F, typename L, typename A>
struct restBinder
{
    restBinder(const F fun, const L src, const A arg) :
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
inline struct restBinder<F, L, A> bindRest(const F fun, const L src, const A arg)
{
    return restBinder<F, L, A>(fun, src, arg);
}

template<typename F, typename L, typename A>
struct restPairBinder
{
    restPairBinder(const F fun, const L lft, const L rgt, const A arg) :
        fun(fun), lft(lft), rgt(rgt), arg(arg)
    {
    }

    const L operator() () const
    {
        return fun(lft.rest(), rgt.rest(), arg);
    }

private:
    const F fun;
    const L lft;
    const L rgt;
    const A arg;
};

template<typename F, typename L, typename A>
inline struct restPairBinder<F, L, A>
bindRest(const F fun, const L lft, const L rgt, const A arg)
{
    return restPairBinder<F, L, A>(fun, lft, rgt, arg);
}


template<typename L>
L getRest(const L list)
{
    return list.rest();
}

template<typename L, typename Functor>
inline void forEach(const L list, const Functor f)
{
    for (L p = list; !p.isEmpty(); p = p.rest())
    {
        f(p.first());
    }
}

template<typename L, typename F>
L mapList(const L src, const F fun)
{
    if (src.isEmpty())
    {
        return src;
    }
    else
    {
        return makeList(fun(src.first()), bindRest(mapList<L, F>, src, fun));
        // return makeList(fun(src.first()),
        //                 curry(compose(mapList<L, F>,
        //                               curry(getRest<L>, src)),
        //                       fun));
    }
}

template<typename L, typename F>
L zipLists(const L lft, const L rgt, const F fun)
{
    if (lft.isEmpty() or rgt.isEmpty())
    {
        return L();
    }
    else
    {
        return makeList(fun(lft.first(), rgt.first()),
                        bindRest(zipLists<L, F>, lft, rgt, fun));
    }
}

template<typename L>
L operator+(const L lft, const L rgt)
{
    return zipLists(lft, rgt, std::plus<typename L::element_type>());
}

template<typename L>
L operator-(const L lft, const L rgt)
{
    return zipLists(lft, rgt, std::minus<typename L::element_type>());
}

template<typename L>
L operator*(const L lft, const L rgt)
{
    return zipLists(lft, rgt, std::multiplies<typename L::element_type>());
}

template<typename L>
L operator/(const L lft, const L rgt)
{
    return zipLists(lft, rgt, std::divides<typename L::element_type>());
}

template<typename L, typename F>
L filterList(const L src, const F pred)
{
    L p = src;
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
        return makeList(p.first(), bindRest(filterList<L, F>, p, pred));
    }
}

template<typename L>
L takeList(const L list, const int n)
{
    if (list.isEmpty() or n <= 0)
    {
        return L();
    }
    else
    {
        return makeList(list.first(), bindRest(takeList<L>, list, n-1));
    }
}

template<typename L>
L dropList(const L list, const int n)
{
    L p = list;
    int i = n;
    while (not (p.isEmpty() or i <= 0))
    {
        --i;
        p = p.rest();
    }

    return p;
}

template<typename T>
List<T> listFrom(const T start)
{
    return makeList(start, curry(listFrom<T>, start + 1));
}

}

#endif // !ODF_LIST_FUN_HPP
