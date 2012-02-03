#include <iostream>
#include <string>
#include <boost/spirit/include/phoenix.hpp>
#include "list_fun.hpp"

using namespace odf;
using namespace boost::phoenix;
using namespace boost::phoenix::arg_names;

void print(int val, std::string pre, std::string post)
{
    std::cout << pre << val << post;
}

int main()
{
    List<int> three = cons(3, cons(2, cons(1)));
    List<int> four  = cons(4, val(three.rest()));

    std::cout << three << std::endl;
    std::cout << four << std::endl;

    forEach(three, bind(print, arg1, "<", "> "));
}
