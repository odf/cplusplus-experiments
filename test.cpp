#include <iostream>
#include <string>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include "LinkedList.hpp"
#include "list_fun.hpp"

using namespace std;
using namespace boost::lambda;
using namespace odf;

template<typename T>
void print(T val, string pre, string post)
{
    cout << pre << val << post;
}

template<typename L>
struct fibBinder
{
    fibBinder(const L& list) :
        list(list)
    {
    }

    const L operator() () const
    {
        return list + list.rest();
    }

private:
    const L& list;
};

template<typename L>
inline struct fibBinder<L> bindFib(const L& list)
{
    return fibBinder<L>(list);
}

int main()
{
    List<int> three = makeList(3, makeList(2, makeList(1)));
    List<int> four  = makeList(4, constant(three.rest()));

    cout << three << endl;
    cout << four << endl;

    forEach(three, bind(print<int>, _1, "<", "> "));
    cout << endl;

    cout << mapList(four, _1 * 2) << endl;

    cout << filterList(three, _1 % 2 == 1) << endl;

    cout << three + four << endl;

    cout << three - four.rest() * three << endl;

    // DISCLAIMER: The following looks nifty, but creates a memory hole due to
    //             circular references, which shared_ptr can't handle.
    List<int> fib = makeList(0, makeList(1, bindFib(fib)));
    cout << takeList(fib, 10) << endl;
    cout << takeList(dropList(fib, 10), 10) << endl;
}
