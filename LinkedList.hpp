#ifndef ODF_LINKEDLIST_HPP
#define ODF_LINKEDLIST_HPP 1

#include <tr1/memory>
#include "Thunk.hpp"

namespace odf
{

template<typename T>
class ListLink;

template<typename T>
class List : public std::tr1::shared_ptr<ListLink<T> >
{
};

template<typename T>
class ListLink
{
public:
    virtual const T first() = 0;
    virtual List<T> rest() const = 0;
};

template<typename T, typename Functor = T(*)()>
class ListLinkImpl : public ListLink<T>
{
protected:
    typedef Thunk<T, Functor> Data;

    Data content_;
    List<T> rest_;
    List<T> self_;

public:
    ListLinkImpl(Data content, List<T> rest) :
        content_(content),
        rest_(rest),
        self_()
    {
    }
    
    ListLinkImpl(Data content) :
        content_(content),
        rest_(),
        self_()
    {
    }

    const T first()
    {
        return content_();
    }

    List<T> rest() const
    {
        return rest_;
    }

    List<T> operator&()
    {
        if (self_.get() == 0)
        {
            ListLinkImpl *self = new ListLinkImpl(*this);
            self->self_ = self_;
            self_.reset(self);
        }
        return self_;
    }
};

template<typename T, typename Functor>
List<T> makeList(const Functor code)
{
    return &ListLinkImpl<T, Functor>(code);
}

template<typename T, typename Functor>
List<T> makeList(const Functor code, const List<T> rest)
{
    return &ListLinkImpl<T, Functor>(code, rest);
}

}

#endif // !ODF_LINKEDLIST_HPP
