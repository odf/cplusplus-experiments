#ifndef ODF_LINKEDLIST_HPP
#define ODF_LINKEDLIST_HPP 1

#include "Thunk.hpp"

namespace odf
{

template<typename T>
class List
{
private:
    typedef Thunk<List<T> > Ptr;

    Ptr next_;
    bool is_empty_;
    T first_;

public:
    typedef T element_type;

    List() :
        next_(),
        is_empty_(true),
        first_()
    {
    }

    List(const T first, const Ptr next) :
        next_(next),
        is_empty_(false),
        first_(first)
    {
    }
    
    explicit List(const T first) :
        next_(),
        is_empty_(false),
        first_(first)
    {
    }

    const T first() const
    {
        return first_;
    }
    
    const bool isEmpty() const
    {
        return is_empty_;
    }

    const List rest() const
    {
        if (next_.isEmpty())
        {
            return List();
        }
        else
        {
            return next_();
        }
    }
};

template<typename T>
inline List<T> makeList(const T first)
{
    return List<T>(first);
}

template<typename T>
inline List<T> makeList(const T first, const List<T> rest)
{
    return List<T>(first, makeExplicitThunk(rest));
}

template<typename T, typename Functor>
inline List<T> makeList(const T first, const Functor code)
{
    return List<T>(first, makeThunk<List<T> >(code));
}


template<typename T>
std::ostream& operator<<(std::ostream& out, const List<T> list)
{
    if (!list.isEmpty())
    {
        out << list.first();
        for (List<T> p = list.rest(); !p.isEmpty(); p = p.rest())
        {
            out << " " << p.first();
        }
    }
    return out;
}

}

#endif // !ODF_LINKEDLIST_HPP
