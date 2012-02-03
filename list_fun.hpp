#ifndef ODF_LIST_FUN_HPP
#define ODF_LIST_FUN_HPP 1

#include <iostream>
#include "LinkedList.hpp"

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
void forEach(List<T> list, const Functor f)
{
    List<T> p = list;
    while (!p.isEmpty()) {
        T val = p.first();
        f(val);
        p = p.rest();
    }
}

template<typename T>
std::ostream& operator<<(std::ostream& out, List<T> list)
{
    if (!list.isEmpty()) {
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
