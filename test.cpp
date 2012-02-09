#include <algorithm>
#include <iostream>
#include <string>
#include "Integer.h"
#include "LinkedList.hpp"
#include "ListIterator.hpp"
#include "fun.hpp"
#include "list_fun.hpp"

using namespace std;
using namespace odf;

void print(const int val)
{
    cout << "<" << val << ">";
}

int mul(const int lft, const int rgt)
{
    return lft * rgt;
}

bool isOdd(const int n)
{
    return n % 2 == 1;
}

List<Integer> fibonacci(const Integer a, const Integer b)
{
    return makeList(a, curry2(fibonacci, b, a + b));
}

int main()
{
    List<int> three = makeList(3, makeList(2, makeList(1)));
    List<int> four  = makeList(4, (three.*(&List<int>::rest))());

    cout << three << endl;
    cout << four << endl;

    forEach(three, print);
    cout << endl;

    std::for_each(ListIterator<int>(four), ListIterator<int>(), print);
    cout << endl;

    cout << mapList(four, curry(mul, 2)) << endl;

    cout << filterList(three, isOdd) << endl;

    cout << three + four << endl;

    cout << three - four.rest() * three << endl;

    cout << takeList(listFrom(1000), 10) << endl;

    List<Integer> fib = fibonacci(Integer(0), Integer(1));
    cout << takeList(fib, 20) << endl;
    cout << takeList(dropList(fib, 200), 10) << endl;
}
