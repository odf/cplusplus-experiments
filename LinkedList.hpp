#ifndef ODF_LINKEDLIST_HPP
#define ODF_LINKEDLIST_HPP

#include <tr1/memory>

namespace odf
{


template<typename T>
class LinkedList : public std::tr1::shared_ptr<T>
{
protected:
    class LinkedListIterator
    {
    private:
        LinkedList &current_;
        friend class LinkedList;

        LinkedListIterator(LinkedList &list) :
            current_(list)
        {
        }

    public:
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

    LinkedList(const LinkedList *next) :
        Ptr(),
        next_(next)
    {
    }

public:
    LinkedList(const T value, const LinkedList *next) :
        Ptr(new T(value)),
        next_(next)
    {
    }
    
    LinkedList(const T value) :
        Ptr(new T(value)),
        next_(0)
    {
    }

    LinkedList() :
        Ptr(),
        next_(0)
    {
    }

    const LinkedList next() const
    {
        return next_ ? *next_ : LinkedList();
    }

    const bool operator==(const LinkedList other) const
    {
        return Ptr::get() == other.Ptr::get()
            && (void *) next_ == (void *) other.next_;
    }

    const bool operator!=(const LinkedList other) const
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
