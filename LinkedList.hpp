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
            self_.reset(new LinkedList(*this));
            self_->self_ = self_;
        }
        return self_;
    }
};

}

#endif // !ODF_LINKEDLIST_HPP
