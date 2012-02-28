/** -*-c++-*-
 *
 *  Hash array mapped tries (HAMT) as introduced by Phil Bagwell.
 *
 *  Copyright 2012  Olaf Delgado-Friedrichs
 *
 */


#ifndef ODF_HASH_TRIE_HPP
#define ODF_HASH_TRIE_HPP 1

#include <cstring>
#include <stdint.h>
#include <vector>

#include <boost/config.hpp>

#ifdef BOOST_HAS_TR1_SHARED_PTR
#include <tr1/memory>
#else
#include <boost/tr1/memory.hpp>
#endif

#ifdef BOOST_HAS_TR1_HASH
#include <functional>
#else
#include <boost/tr1/functional.hpp>
#endif


namespace odf
{
namespace hash_trie
{

typedef uint32_t hashType;
typedef uint8_t  indexType;


// ----------------------------------------------------------------------------
// Bit counting and manipulation functions.
// ----------------------------------------------------------------------------

inline
hashType mask(hashType const n, indexType const shift)
{
    return (n >> shift) & 0x1f;
}

inline
indexType bitCount(hashType n)
{
    n -= (n >> 1) & 0x55555555;
    n = (n & 0x33333333) + ((n >> 2) & 0x33333333);
    n = (n & 0x0f0f0f0f) + ((n >> 4) & 0x0f0f0f0f);
    n += n >> 8;
    return (n + (n >> 16)) & 0x3f;
}

inline
indexType indexForBit(hashType const bitmap, hashType const bit)
{
    return bitCount(bitmap & (bit - 1));
}

inline
hashType maskBit(hashType const n, indexType const shift)
{
    return 1 << mask(n, shift);
}


// ----------------------------------------------------------------------------
// Array copying with small modifications
// ----------------------------------------------------------------------------

template<typename T>
inline
T const* const arrayWith(T const* const source, int const len,
                         int const pos, T const val)
{
    T* copy = new T[len];
    memcpy(copy, source, len * sizeof(T));
    copy[pos] = val;
    return copy;
}

template<typename T>
inline
T const* const arrayWithInsertion(T const* const source, int const len,
                                  int const pos, T const val)
{
    T* copy = new T[len + 1];
    memcpy(copy, source, pos * sizeof(T));
    copy[pos] = val;
    memcpy(&copy[pos + 1], &source[pos], (len - pos) * sizeof(T));
    return copy;
}

template<typename T>
inline
T const* const arrayWithout(T const* const source, int const len,
                            int const pos)
{
    T* copy = new T[len - 1];
    memcpy(copy, source, pos * sizeof(T));
    memcpy(&copy[pos], &source[pos + 1], (len - pos - 1) * sizeof(T));
    return copy;
}


// ----------------------------------------------------------------------------
// We need some forward declarations for hash trie node type
// ----------------------------------------------------------------------------

template<typename Key, typename Val> class Leaf;
template<typename Key, typename Val> class CollisionNode;
template<typename Key, typename Val> class BitmappedNode;

// ----------------------------------------------------------------------------
// The interface for nodes of the hash trie data structure
// ----------------------------------------------------------------------------

template<typename Key, typename Val>
class Node
{
    typename std::tr1::shared_ptr<Val>             typedef ValPtr;
    typename std::tr1::shared_ptr<Node<Val, Key> > typedef NodePtr;
    typename std::tr1::shared_ptr<Leaf<Val, Key> > typedef LeafPtr;

    virtual ~Node() = 0;

    virtual size_t size() const;

    virtual ValPtr get(indexType const shift,
                       hashType  const hash,
                       Key       const key) const;

    virtual NodePtr insert(NodePtr   const self,
                           indexType const shift,
                           hashType  const hash,
                           LeafPtr   const leaf) const;

    virtual NodePtr erase(NodePtr   const self,
                          indexType const shift,
                          hashType  const hash,
                          Key       const key) const;
};

// ----------------------------------------------------------------------------
// A leaf node holds a single key,value pair
// ----------------------------------------------------------------------------

template<typename Key, typename Val>
class Leaf : public Node<Key, Val>
{
    typename Node<Key, Val>::ValPtr  typedef ValPtr;
    typename Node<Key, Val>::NodePtr typedef NodePtr;
    typename Node<Key, Val>::LeafPtr typedef LeafPtr;

    Leaf(hashType const hash, Key const key, ValPtr const value)
        : hash_(hash),
          key_(key),
          value_(value)
    {
    }

    size_t size() const { return 1; }

    ValPtr get(indexType const shift,
               hashType  const hash,
               Key       const key) const
    {
        return key == key_ ? value_ : ValPtr();
    }

    NodePtr insert(NodePtr   const self,
                   indexType const shift,
                   hashType  const hash,
                   LeafPtr   const leaf) const
    {
        if (key_ == leaf.key_)
        {
            return leaf;
        }
        else
        {
            NodePtr base;
            if (hash_ == hash)
                base = NodePtr(new CollisionNode<Key, Val>(hash));
            else
                base = NodePtr(new BitmappedNode<Key, Val>());

            NodePtr tmp = base->insert(base, shift, hash_, self);
            return tmp->insert(tmp, shift, hash, leaf);
        }
    }

    NodePtr erase(NodePtr   const self,
                  indexType const shift,
                  hashType  const hash,
                  Key       const key) const
    {
        return NodePtr();
    }

    Key key() const { return key_; }

private:
    hashType const hash_;
    Key const key_;
    ValPtr const value_;
};

// ----------------------------------------------------------------------------
// A collision node holds several leaf nodes with equal hash codes.
// ----------------------------------------------------------------------------

template<typename Key, typename Val>
class CollisionNode : public Node<Key, Val>
{
    typename Node<Key, Val>::ValPtr  typedef ValPtr;
    typename Node<Key, Val>::NodePtr typedef NodePtr;
    typename Node<Key, Val>::LeafPtr typedef LeafPtr;

    typedef std::vector<LeafPtr> Bucket;

    CollisionNode(hashType const hash, Bucket const bucket)
        : hash_(hash),
          bucket_(bucket)
    {
    }

    size_t size() const { bucket_.size(); }

    ValPtr get(indexType const shift,
               hashType  const hash,
               Key       const key) const
    {
        for (typename Bucket::const_iterator iter = bucket_.begin();
             iter != bucket_.end();
             ++iter)
        {
            if (iter->key_ == key)
                return iter->get(shift, hash, key);
        }
        return ValPtr();
    }

    NodePtr insert(NodePtr   const self,
                   indexType const shift,
                   hashType  const hash,
                   LeafPtr   const leaf) const
    {
        if (hash != hash_)
        {
            NodePtr base = NodePtr(new BitmappedNode<Key, Val>());
            NodePtr tmp = base->insert(base, shift, hash_, self);
            return tmp->insert(tmp, shift, hash, leaf);
        }
        else
        {
            Bucket newBucket = bucketWithout(leaf.key());
            newBucket.push_back(leaf);
            return NodePtr(new CollisionNode(hash, newBucket));
        }
    }

    NodePtr erase(NodePtr   const self,
                  indexType const shift,
                  hashType  const hash,
                  Key       const key) const
    {
        if (bucket_.size() < 2)
        {
            return NodePtr();
        }
        else if (bucket_.size() == 2)
        {
            if (bucket_.at(0)->key() != key)
                return bucket_.at(0);
            else
                return bucket_.at(1);
        }
        else
        {
            return NodePtr(new CollisionNode(hash, bucketWithout(key)));
        }
    }

private:
    hashType const hash_;
    Bucket const bucket_;

    Bucket bucketWithout(Key const key) const
    {
        Bucket result;
        for (typename Bucket::const_iterator iter = bucket_.begin();
             iter != bucket_.end();
             ++iter)
        {
            if (iter->key_ != key)
                result.push_back(*iter);
        }
        return result;
    }
};

}
}

#endif // !ODF_HASH_TRIE_HPP
