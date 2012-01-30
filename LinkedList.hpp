#ifndef ODF_LINKEDLIST_HPP
#define ODF_LINKEDLIST_HPP

#include <tr1/memory>

namespace odf
{

template<typename T>
class LinkedList : public std::tr1::shared_ptr<T>
{
private:
    typedef std::tr1::shared_ptr<T> Ptr;

    const LinkedList *next_;

public:
    LinkedList<T>(const T value, const LinkedList *next) :
    Ptr(new T(value)), next_(next)
    {
    }
    
    LinkedList<T>(const T value) :
    Ptr(new T(value)), next_(0)
    {
    }

    LinkedList<T>() :
    Ptr(), next_(0)
    {
    }

    const LinkedList<T> next() const
    {
        return next_ ? *next_ : LinkedList();
    }
};

}

#endif // !ODF_LINKEDLIST_HPP
