#ifndef ODF_LIST_FUN_HPP
#define ODF_LIST_FUN_HPP 1


namespace odf
{

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

}

#endif // !ODF_LIST_FUN_HPP
