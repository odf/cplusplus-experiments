#ifndef ODF_LINKEDLIST_HPP
#define ODF_LINKEDLIST_HPP 1

#include <tr1/memory>
#include "Thunk.hpp"

namespace odf
{

template<typename T, typename Functor = T(*)()>
class LinkedList
{
protected:
    typedef std::tr1::shared_ptr<LinkedList> Ptr;

    Functor code_;
    T value_;
    bool resolved_;
    Ptr next_;
    Ptr self_;

public:
    LinkedList(Functor code, Ptr next) :
        code_(code),
        resolved_(false),
        next_(next),
        self_(Ptr())
    {
    }
    
    LinkedList(Functor code) :
        code_(code),
        resolved_(false),
        next_(Ptr()),
        self_(Ptr())
    {
    }

    LinkedList(const T value, Ptr next) :
        value_(value),
        resolved_(true),
        next_(next),
        self_(Ptr())
    {
    }
    
    LinkedList(const T value) :
        value_(value),
        resolved_(true),
        next_(Ptr()),
        self_(Ptr())
    {
    }

    const T value()
    {
        if (!resolved_)
        {
            value_ = code_();
            resolved_ = true;
        }
        return value_;
    }

    Ptr next() const
    {
        return next_;
    }

    Ptr operator&()
    {
        if (self_.get() == 0)
        {
            self_.reset(new LinkedList(*this));
        }
        return self_;
    }
};

}

#endif // !ODF_LINKEDLIST_HPP
