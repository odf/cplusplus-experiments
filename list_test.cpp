#include <algorithm>
#include <iostream>
#include <string>
#include "Integer.h"
#include "List.hpp"
#include "fun.hpp"
#include "list_fun.hpp"

using namespace std;
using namespace odf;

void print(const int val)
{
    cout << "<" << val << ">";
}

template<typename T>
T mul(const T lft, const T rgt)
{
    return lft * rgt;
}

bool isOdd(const int n)
{
    return n % 2 == 1;
}

List<Integer> fibonacci(const Integer a, const Integer b)
{
    return makeList(a, curry(fibonacci, b, a + b));
}

template<typename T, std::size_t N>
inline std::size_t length(const T(&)[N])
{
    return N;
}


int main()
{
    double a[] = { 3.14, 2.72, 1.23 };
    List<int> three = makeList(3, makeList(2, makeList(1)));
    List<int> four  = makeList(4, (three.*(&List<int>::rest))());

    cout << three << endl;
    cout << four << endl;

    cout << arraySlice(a, 1, length(a)) << endl;
    cout << asList(a) << endl;
    cout << reduceList(asList(a), mul<double>) << endl;
    cout << sum(asList(a)) << endl;

    forEach(three, print);
    cout << endl;

    std::for_each(four.begin(), four.end(), print);
    cout << endl;

    cout << mapList(four, compose(curry(mul<int>, 2), curry(mul<int>, 3)))
         << endl;

    cout << filterList(three, isOdd) << endl;

    cout << three + four << endl;

    cout << three - four.rest() * three << endl;

    cout << takeList(listFrom(1000), 10) << endl;

    List<Integer> fib = fibonacci(Integer(0), Integer(1));
    cout << takeList(fib, 20) << endl;
    cout << takeList(dropList(fib, 200), 10) << endl;
}
