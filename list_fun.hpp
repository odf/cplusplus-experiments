#ifndef ODF_LIST_FUN_HPP
#define ODF_LIST_FUN_HPP 1

#include <functional>

namespace odf
{

template<typename Function> struct function_traits;

template<typename R>
struct function_traits<R(*)()>
{
    static const std::size_t arity = 0;
    typedef R                result_type;
};

template<typename R, typename P>
struct function_traits<R(*)(P)>
{
    static const std::size_t arity = 1;
    typedef R                result_type;
    typedef P                arg1_type;
};

template<typename R, typename P, typename Q>
struct function_traits<R(*)(P, Q)>
{
    static const std::size_t arity = 2;
    typedef R                result_type;
    typedef P                arg1_type;
    typedef Q                arg2_type;
};


template<typename F>
struct unaryBinder
{
    typename function_traits<F>::arg1_type   typedef A;
    typename function_traits<F>::result_type typedef R;

    unaryBinder(const F fun, const A arg) :
        fun(fun), arg(arg)
    {
    }

    const R operator() () const
    {
        return fun(arg);
    }

private:
    const F fun;
    const A arg;
};

template<typename F>
inline struct unaryBinder<F> bindFunction(
    const F fun,
    const typename function_traits<F>::arg1_type arg)
{
    return unaryBinder<F>(fun, arg);
}

template<typename F>
struct binaryBinder
{
    typename function_traits<F>::arg1_type   typedef A;
    typename function_traits<F>::arg2_type   typedef B;
    typename function_traits<F>::result_type typedef R;

    binaryBinder(const F fun, const A arg1, const B arg2) :
        fun(fun), arg1(arg1), arg2(arg2)
    {
    }

    const R operator() () const
    {
        return fun(arg1, arg2);
    }

private:
    const F fun;
    const A arg1;
    const B arg2;
};

template<typename F>
inline struct binaryBinder<F> bindFunction(
    const F fun,
    const typename function_traits<F>::arg1_type arg1,
    const typename function_traits<F>::arg2_type arg2)
{
    return binaryBinder<F>(fun, arg1, arg2);
}

template<typename F>
struct binaryCurrier
{
    typename function_traits<F>::arg1_type   typedef A;
    typename function_traits<F>::arg2_type   typedef B;
    typename function_traits<F>::result_type typedef R;

    binaryCurrier(const F fun, const A arg1) :
        fun(fun), arg1(arg1)
    {
    }

    const R operator() (const B arg2) const
    {
        return fun(arg1, arg2);
    }

private:
    const F fun;
    const A arg1;
};

template<typename F>
inline struct binaryCurrier<F> curryFunction(
    const F fun,
    const typename function_traits<F>::arg1_type arg1)
{
    return binaryCurrier<F>(fun, arg1);
}

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
    return makeList(start, bindFunction(listFrom<T>, start + 1));
}

}

#endif // !ODF_LIST_FUN_HPP
