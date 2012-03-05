/* -*-c++-*- */

#include <string>
#include <sstream>
#include <unittest++/UnitTest++.h>

#include "Integer.h"
#include "List.hpp"
#include "fun.hpp"
#include "list_fun.hpp"

using namespace odf;

template<typename T>
std::string asString(T const x)
{
    std::stringstream ss;
    ss << x;
    return ss.str();
}

template<typename S, typename T>
void CHECK_EQUAL_LISTS(List<S> const left, List<T> const right)
{
    List<S> pl;
    List<T> pr;

    for (pl = left, pr = right;
         not pl.isEmpty() and not pr.isEmpty();
         pl = pl.rest(), pr = pr.rest())
    {
        CHECK_EQUAL(pl.first(), pr.first());
    }
    CHECK_EQUAL(pl.isEmpty() ? "<END>" : asString(pl.first()),
                pr.isEmpty() ? "<END>" : asString(pr.first()));
}

template<typename S, typename T,  std::size_t N>
void CHECK_EQUAL_LISTS(const S(&left)[N], List<T> const right)
{
    CHECK_EQUAL_LISTS(arraySlice<S>(left, 0, N), right);
}


SUITE(Fibonacci)
{
    template<typename T>
    List<T> fibonacci(T const a, T const b)
    {
        return makeList(a, curry(fibonacci<T>, b, a + b));
    }

    TEST(First15)
    {
        List<Integer> fib = fibonacci(Integer(0), Integer(1));
        int fib15[] = { 0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377 };
        
        CHECK_EQUAL_LISTS(fib15, takeList(fib, 15));
    }
}


int main()
{
    return UnitTest::RunAllTests();
}
