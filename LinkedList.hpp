#ifndef ODF_LINKEDLIST_HPP
#define ODF_LINKEDLIST_HPP

#include <tr1/memory>

namespace odf
{

template<typename T>
class LinkedList
{
protected:
    typedef T(*FunPtr)();
    typedef std::tr1::shared_ptr<LinkedList> Ptr;

    FunPtr code_;
    T value_;
    Ptr next_;
    Ptr self_;

public:
    LinkedList(FunPtr code, Ptr next) :
        code_(code),
        next_(next),
        self_(Ptr())
    {
    }
    
    LinkedList(FunPtr code) :
        code_(code),
        next_(Ptr()),
        self_(Ptr())
    {
    }

    LinkedList(const T value, Ptr next) :
        value_(value),
        code_(0),
        next_(next),
        self_(Ptr())
    {
    }
    
    LinkedList(const T value) :
        value_(value),
        code_(0),
        next_(Ptr()),
        self_(Ptr())
    {
    }

    const T value()
    {
        if (code_ != 0)
        {
            value_ = (*code_)();
            code_ = 0;
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
            LinkedList *l = new LinkedList(code_, next_);
            l->value_ = value_;
            l->self_ = self_;
            self_.reset(l);
        }
        return self_;
    }
};

}

#endif // !ODF_LINKEDLIST_HPP
