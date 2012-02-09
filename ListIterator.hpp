#ifndef ODF_LISTITERATOR_HPP
#define ODF_LISTITERATOR_HPP 1

#include <boost/iterator/iterator_facade.hpp>

#include "LinkedList.hpp"

namespace odf
{

template<typename T>
class ListIterator
    : public boost::iterator_facade<ListIterator<T>,
                                    const List<T>,
                                    boost::forward_traversal_tag,
                                    T>
{
 public:
    ListIterator()
      : list_()
    {
    }

    explicit ListIterator(const List<T>& list)
      : list_(list)
    {
    }

 private:
    friend class boost::iterator_core_access;

    void increment()
    {
        list_ = list_.rest();
    }

    const bool equal(const ListIterator& other) const
    {
        return this->list_ == other.list_;
    }

    const T dereference() const
    {
        return list_.first();
    }

    List<T> list_;
};

}

#endif // !ODF_LISTITERATOR_HPP
