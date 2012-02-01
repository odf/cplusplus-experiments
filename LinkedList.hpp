#ifndef ODF_LINKEDLIST_HPP
#define ODF_LINKEDLIST_HPP 1

#include <tr1/memory>
#include "Thunk.hpp"

namespace odf
{

template<typename T>
class AbstractListLink;

template<typename T>
class List : public std::tr1::shared_ptr<AbstractListLink<T> >
{
};

template<typename T>
class AbstractListLink
{
public:
    virtual const T first() = 0;
    virtual List<T> rest() const = 0;
};

template<typename T, typename Functor = T(*)()>
class ListLink : public AbstractListLink<T>
{
protected:
    typedef Thunk<T, Functor> Data;

    Data content_;
    List<T> rest_;
    List<T> self_;

public:
    ListLink(Data content, List<T> rest) :
        content_(content),
        rest_(rest),
        self_()
    {
    }
    
    ListLink(Data content) :
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
            ListLink *self = new ListLink(*this);
            self->self_ = self_;
            self_.reset(self);
        }
        return self_;
    }
};

template<typename T, typename Functor>
List<T> makeList(const Functor code)
{
    return &ListLink<T, Functor>(code);
}

template<typename T, typename Functor>
List<T> makeList(const Functor code, const List<T> rest)
{
    return &ListLink<T, Functor>(code, rest);
}

}

#endif // !ODF_LINKEDLIST_HPP
