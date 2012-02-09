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


}

#endif // !ODF_FUN_HPP
