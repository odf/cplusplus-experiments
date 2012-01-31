#ifndef ODF_LINKEDLIST_HPP
#define ODF_LINKEDLIST_HPP 1

#include <tr1/memory>
#include "Thunk.hpp"

namespace odf
{

template<typename T>
class AbstractList
{
public:
    typedef std::tr1::shared_ptr<AbstractList<T> > Ptr;
    virtual const T value() = 0;
    virtual Ptr next() const = 0;
};


template<typename T, typename Functor = T(*)()>
class LinkedList : public AbstractList<T>
{
public:
    typename AbstractList<T>::Ptr typedef Ptr;

protected:
    typedef Thunk<T, Functor> Data;

    Data content_;
    Ptr next_;
    Ptr self_;

public:
    LinkedList(Data content, Ptr next) :
        content_(content),
        next_(next),
        self_()
    {
    }
    
    LinkedList(Data content) :
        content_(content),
        next_(),
        self_()
    {
    }

    const T value()
    {
        return content_();
    }

    Ptr next() const
    {
        return next_;
    }

    Ptr operator&()
    {
        if (self_.get() == 0)
        {
            LinkedList *self = new LinkedList(*this);
            self->self_ = self_;
            self_.reset(self);
        }
        return self_;
    }
};

template<typename T, typename Functor>
LinkedList<T, Functor> makeList(const Functor code)
{
    return LinkedList<T, Functor>(code);
}

template<typename T, typename Functor>
LinkedList<T, Functor> makeList(const Functor code,
                                const typename AbstractList<T>::Ptr next)
{
    return LinkedList<T, Functor>(code, next);
}

}

#endif // !ODF_LINKEDLIST_HPP
