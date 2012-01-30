#ifndef ODF_LINKEDLIST_HPP
#define ODF_LINKEDLIST_HPP

#include <tr1/memory>

namespace odf
{


template<typename T>
class LinkedList : public std::tr1::shared_ptr<T>
{
private:
    class LinkedListIterator
    {
    private:
        LinkedList<T> &current_;

    public:
        LinkedListIterator(LinkedList<T> &list) :
            current_(list)
        {
        }

        LinkedListIterator operator++()
        {
            current_ = current_.next();
        }

        const T operator*() const
        {
            return *current_;
        }

        const bool operator!=(const LinkedListIterator other) const
        {
            return current_ != other.current_;
        }
    };

    typedef std::tr1::shared_ptr<T> Ptr;

    const LinkedList *next_;

public:
    LinkedList<T>(const T value, const LinkedList *next) :
        Ptr(new T(value)),
        next_(next)
    {
    }
    
    LinkedList<T>(const T value) :
        Ptr(new T(value)),
        next_(0)
    {
    }

    LinkedList<T>() :
        Ptr(),
        next_(0)
    {
    }

    const LinkedList<T> next() const
    {
        return next_ ? *next_ : LinkedList();
    }

    const bool operator==(const LinkedList<T> other) const
    {
        return Ptr::get() == other.Ptr::get()
            && (void *) next_ == (void *) other.next_;
    }

    const bool operator!=(const LinkedList<T> other) const
    {
        return Ptr::get() != other.Ptr::get()
            || (void *) next_ != (void *) other.next_;
    }

    LinkedListIterator begin()
    {
        return (LinkedListIterator) *this;
    }

    LinkedListIterator end()
    {
        static LinkedList empty;
        return (LinkedListIterator) empty;
    }
};

}

#endif // !ODF_LINKEDLIST_HPP
