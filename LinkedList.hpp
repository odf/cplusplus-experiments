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

    FunPtr code_;
    T value_;
    LinkedList *next_;

public:
    LinkedList(FunPtr code, LinkedList *next) :
        code_(code),
        next_(next)
    {
    }
    
    LinkedList(FunPtr code) :
        code_(code),
        next_(0)
    {
    }

    LinkedList(const T value, LinkedList *next) :
        value_(value),
        code_(0),
        next_(next)
    {
    }
    
    LinkedList(const T value) :
        value_(value),
        code_(0),
        next_(0)
    {
    }

    LinkedList() :
        code_(0),
        next_(0)
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

    LinkedList *next() const
    {
        return next_;
    }

    const bool operator==(const LinkedList &other) const
    {
        return **this == *other && (void *) next_ == (void *) other.next_;
    }

    const bool operator!=(const LinkedList &other) const
    {
        return **this != *other || (void *) next_ != (void *) other.next_;
    }
};

}

#endif // !ODF_LINKEDLIST_HPP
