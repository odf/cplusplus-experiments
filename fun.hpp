#ifndef ODF_FUN_HPP
#define ODF_FUN_HPP 1

namespace odf
{

template<typename Function> struct function_traits;

template<typename R>
struct function_traits<R(*)()>
{
    static const std::size_t arity = 0;
    typedef R                result_type;
};

template<typename F>
struct unaryCurrier
{
    typename function_traits<F>::result_type typedef R;
    typename function_traits<F>::arg1_type   typedef A;

    unaryCurrier(const F fun, const A arg) :
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
struct binaryCurrier
{
    typename function_traits<F>::result_type typedef R;
    typename function_traits<F>::arg1_type   typedef A;
    typename function_traits<F>::arg2_type   typedef B;

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
struct ternaryCurrier
{
    typename function_traits<F>::result_type typedef R;
    typename function_traits<F>::arg1_type   typedef A;
    typename function_traits<F>::arg2_type   typedef B;
    typename function_traits<F>::arg3_type   typedef C;

    ternaryCurrier(const F fun, const A arg1) :
        fun(fun), arg1(arg1)
    {
    }

    const R operator() (const B arg2, const C arg3) const
    {
        return fun(arg1, arg2, arg3);
    }

private:
    const F fun;
    const A arg1;
};

template<typename R, typename A>
struct function_traits<R(*)(A)>
{
    static const std::size_t arity = 1;

    typedef struct unaryCurrier<R(*)(A)> currier_type;

    typedef R result_type;
    typedef A arg1_type;
};

template<typename R, typename A, typename B>
struct function_traits<R(*)(A, B)>
{
    static const std::size_t arity = 2;

    typedef struct binaryCurrier<R(*)(A, B)> currier_type;

    typedef R result_type;
    typedef A arg1_type;
    typedef B arg2_type;
};

template<typename R, typename A, typename B, typename C>
struct function_traits<R(*)(A, B, C)>
{
    static const std::size_t arity = 3;

    typedef struct binaryCurrier<R(*)(A, B, C)> currier_type;

    typedef R result_type;
    typedef A arg1_type;
    typedef B arg2_type;
    typedef C arg3_type;
};

template<typename F>
struct function_traits<unaryCurrier<F> >
{
    static const std::size_t arity = 0;

    typename function_traits<F>::result_type typedef result_type;
};

template<typename F>
struct function_traits<binaryCurrier<F> >
{
    static const std::size_t arity = 1;

    typedef struct unaryCurrier<binaryCurrier<F> > currier_type;

    typename function_traits<F>::result_type typedef result_type;
    typename function_traits<F>::arg2_type   typedef arg1_type;
};

template<typename F>
struct function_traits<ternaryCurrier<F> >
{
    static const std::size_t arity = 2;

    typedef struct binaryCurrier<ternaryCurrier<F> > currier_type;

    typename function_traits<F>::result_type typedef result_type;
    typename function_traits<F>::arg2_type   typedef arg1_type;
    typename function_traits<F>::arg3_type   typedef arg2_type;
};

template<typename F>
typename function_traits<F>::currier_type curry(
    const F fun,
    const typename function_traits<F>::arg1_type arg)
{
    return typename function_traits<F>::currier_type(fun, arg);
}

template<typename F>
typename function_traits<
    typename function_traits<F>::currier_type>::currier_type
curry2(
    const F fun,
    const typename function_traits<F>::arg1_type arg1,
    const typename function_traits<F>::arg2_type arg2)
{
    return curry(curry(fun, arg1), arg2);
}

}

#endif // !ODF_FUN_HPP
