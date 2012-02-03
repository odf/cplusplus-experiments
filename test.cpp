#include <iostream>
#include <string>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include "list_fun.hpp"

using namespace std;
using namespace boost::lambda;
using namespace odf;

template<typename T>
void print(T val, string pre, string post)
{
    cout << pre << val << post;
}

int main()
{
    List<int> three = cons(3, cons(2, cons(1)));
    List<int> four  = cons(4, constant(three.rest()));

    cout << three << endl;
    cout << four << endl;

    forEach(three, bind(print<int>, _1, "<", "> "));
    cout << endl;

    cout << mapList(four, _1 * 2) << endl;

    cout << filterList(three, _1 % 2 == 1) << endl;

    cout << zipLists(three, four, _1 - _2) << endl;
}
