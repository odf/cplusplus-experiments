#include <iostream>
#include <string>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include "list_fun.hpp"

using namespace std;
using namespace boost::lambda;
using namespace odf;

void print(int val, string pre, string post)
{
    cout << pre << val << post;
}

int main()
{
    List<int> three = cons(3, cons(2, cons(1)));
    List<int> four  = cons(4, constant(three.rest()));

    cout << three << endl;
    cout << four << endl;

    forEach(three, bind(print, _1, "<", "> "));
}
