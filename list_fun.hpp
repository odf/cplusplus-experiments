#ifndef ODF_LIST_FUN_HPP
#define ODF_LIST_FUN_HPP 1

#include <functional>
#include "fun.hpp"

namespace odf
{

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
        return makeList(fun(src.first()),
                        curry(compose(mapList<L, F>, getRest<L>),
                              src, fun));
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
                        curry(compose(curry(compose(zipLists<L, F>,
                                                    getRest<L>),
                                             lft),
                                      getRest<L>),
                              rgt, fun));
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
        return makeList(p.first(),
                        curry(compose(filterList<L, F>, getRest<L>),
                              p, pred));
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
        return makeList(list.first(),
                        curry(compose(takeList<L>, getRest<L>),
                              list, n-1));
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

template<typename L>
L reverseList(const L list)
{
    L result;

    for(L p = list; not p.isEmpty(); p = p.rest())
    {
        result = makeList(p.first(), result);
    }

    return result;
}

template<typename T>
List<T> listFrom(const T start)
{
    return makeList(start, curry(listFrom<T>, start + 1));
}

template<typename Iter>
List<typename Iter::value_type> asList(Iter iter, const Iter end)
{
    List<typename Iter::value_type> result;

    while (iter != end)
    {
        result = makeList(*iter++, result);
    }

    return reverseList(result);
}

template<typename C>
inline List<typename C::value_type> asList(const C& collection)
{
    return asList(collection.begin(), collection.end());
}

template<typename T>
List<T> arraySlice(const T a[], const int from, const int to)
{
    if (from >= to)
    {
        return List<T>();
    }
    else
    {
        return makeList(a[from], curry(arraySlice<T>, a, from+1, to));
    }
}

template<typename T, std::size_t N>
inline List<T> asList(const T(&a)[N])
{
    return arraySlice<T>(a, 0, N);
}

}

#endif // !ODF_LIST_FUN_HPP
