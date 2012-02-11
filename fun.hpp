#ifndef ODF_FUN_HPP
#define ODF_FUN_HPP 1

namespace odf
{

template<typename Function> struct function_traits;

template<typename F>
struct unaryCurrier;

template<typename F>
struct binaryCurrier;

template<typename F>
struct ternaryCurrier;

template<typename Lft, typename Rgt>
struct unaryComposer;

template<typename Lft, typename Rgt>
struct binaryComposer;

template<typename Lft, typename Rgt>
struct ternaryComposer;

template<typename F>
struct nullaryFunctor
{
};

template<typename F>
struct function_traits<nullaryFunctor<F> >
{
    static const std::size_t        arity = 0;
    typename F::result_type typedef result_type;
};

template<typename F>
struct unaryFunctor
{
};

template<typename F>
struct function_traits<unaryFunctor<F> >
{
    static const std::size_t        arity = 1;
    typedef struct unaryCurrier<F>  currier_type;
    typename F::result_type typedef result_type;
    typename F::arg1_type   typedef arg1_type;
};

template<typename F>
struct binaryFunctor
{
};

template<typename F>
struct function_traits<binaryFunctor<F> >
{
    static const std::size_t        arity = 2;
    typedef struct binaryCurrier<F> currier_type;
    typename F::result_type typedef result_type;
    typename F::arg1_type   typedef arg1_type;
    typename F::arg2_type   typedef arg2_type;
};

template<typename F>
struct ternaryFunctor
{
};

template<typename F>
struct function_traits<ternaryFunctor<F> >
{
    static const std::size_t         arity = 3;
    typedef struct ternaryCurrier<F> currier_type;
    typename F::result_type  typedef result_type;
    typename F::arg1_type    typedef arg1_type;
    typename F::arg2_type    typedef arg2_type;
    typename F::arg3_type    typedef arg3_type;
};

template<typename F>
struct unaryCurrier : nullaryFunctor<unaryCurrier<F> >
{
    typename function_traits<F>::result_type typedef result_type;
    typename function_traits<F>::arg1_type   typedef A;

    unaryCurrier(const F fun, const A arg) :
        fun(fun), arg(arg)
    {
    }

    const result_type operator() () const
    {
        return fun(arg);
    }

private:
    const F fun;
    const A arg;
};

template<typename F>
struct function_traits<unaryCurrier<F> > :
        function_traits<nullaryFunctor<unaryCurrier<F> > >
{
};

template<typename F>
struct binaryCurrier : unaryFunctor<binaryCurrier<F> >
{
    typename function_traits<F>::result_type typedef result_type;
    typename function_traits<F>::arg1_type   typedef A;
    typename function_traits<F>::arg2_type   typedef arg1_type;

    binaryCurrier(const F fun, const A arg1) :
        fun(fun), arg1(arg1)
    {
    }

    const result_type operator() (const arg1_type arg2) const
    {
        return fun(arg1, arg2);
    }

private:
    const F fun;
    const A arg1;
};

template<typename F>
struct function_traits<binaryCurrier<F> > :
        function_traits<unaryFunctor<binaryCurrier<F> > >
{
};

template<typename F>
struct ternaryCurrier : binaryFunctor<ternaryCurrier<F> >
{
    typename function_traits<F>::result_type typedef result_type;
    typename function_traits<F>::arg1_type   typedef A;
    typename function_traits<F>::arg2_type   typedef arg1_type;
    typename function_traits<F>::arg3_type   typedef arg2_type;

    ternaryCurrier(const F fun, const A arg1) :
        fun(fun), arg1(arg1)
    {
    }

    const result_type operator() (const arg1_type arg2,
                                  const arg2_type arg3) const
    {
        return fun(arg1, arg2, arg3);
    }

private:
    const F fun;
    const A arg1;
};

template<typename F>
struct function_traits<ternaryCurrier<F> > :
        function_traits<binaryFunctor<ternaryCurrier<F> > >
{
};

template<typename Lft, typename Rgt>
struct unaryComposer : unaryFunctor<unaryComposer<Lft, Rgt> >
{
    typename function_traits<Lft>::result_type typedef result_type;
    typename function_traits<Rgt>::arg1_type   typedef arg1_type;

    unaryComposer(const Lft lft, const Rgt rgt) :
        lft(lft), rgt(rgt)
    {
    }

    const result_type operator() (const arg1_type arg) const
    {
        return lft(rgt(arg));
    }

private:
    const Lft lft;
    const Rgt rgt;
};

template<typename Lft, typename Rgt>
struct function_traits<unaryComposer<Lft, Rgt> > :
        function_traits<unaryFunctor<unaryComposer<Lft, Rgt> > >
{
};

template<typename Lft, typename Rgt>
struct binaryComposer : binaryFunctor<binaryComposer<Lft, Rgt> >
{
    typename function_traits<Lft>::result_type typedef result_type;
    typename function_traits<Rgt>::arg1_type   typedef arg1_type;
    typename function_traits<Lft>::arg2_type   typedef arg2_type;

    binaryComposer(const Lft lft, const Rgt rgt) :
        lft(lft), rgt(rgt)
    {
    }

    const result_type operator() (const arg1_type arg1,
                                  const arg2_type arg2) const
    {
        return lft(rgt(arg1), arg2);
    }

private:
    const Lft lft;
    const Rgt rgt;
};

template<typename Lft, typename Rgt>
struct function_traits<binaryComposer<Lft, Rgt> > :
        function_traits<binaryFunctor<binaryComposer<Lft, Rgt> > >
{
};

template<typename Lft, typename Rgt>
struct ternaryComposer : ternaryFunctor<ternaryComposer<Lft, Rgt> >
{
    typename function_traits<Lft>::result_type typedef result_type;
    typename function_traits<Rgt>::arg1_type   typedef arg1_type;
    typename function_traits<Lft>::arg2_type   typedef arg2_type;
    typename function_traits<Lft>::arg3_type   typedef arg3_type;

    ternaryComposer(const Lft lft, const Rgt rgt) :
        lft(lft), rgt(rgt)
    {
    }

    const result_type operator() (const arg1_type arg1,
                                  const arg2_type arg2,
                                  const arg3_type arg3) const
    {
        return lft(rgt(arg1), arg2, arg3);
    }

private:
    const Lft lft;
    const Rgt rgt;
};

template<typename Lft, typename Rgt>
struct function_traits<ternaryComposer<Lft, Rgt> > :
        function_traits<ternaryFunctor<ternaryComposer<Lft, Rgt> > >
{
};

template<typename R>
struct function_traits<R(*)()>
{
    static const std::size_t arity = 0;
    typedef R                result_type;
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

    typedef struct ternaryCurrier<R(*)(A, B, C)> currier_type;

    typedef R result_type;
    typedef A arg1_type;
    typedef B arg2_type;
    typedef C arg3_type;
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

template<typename Lft, typename Rgt>
struct unaryComposer<Lft, Rgt> compose(const Lft lft, const Rgt rgt)
{
    return unaryComposer<Lft, Rgt>(lft, rgt);
}
    
}

#endif // !ODF_FUN_HPP
