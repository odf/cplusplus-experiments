/* -*-c++-*- */

#ifndef ODF_HASH_TRIE_HPP
#define ODF_HASH_TRIE_HPP 1

#include <cstring>
#include <stdint.h>

namespace odf
{
namespace hash_trie
{

typedef uint32_t keyType;
typedef uint8_t  indexType;

// --------------------------------------------------------------------
// Bit counting and manipulation functions.
// --------------------------------------------------------------------

inline
keyType mask(keyType const n, indexType const shift)
{
    return (n >> shift) & 0x1f;
}

inline
indexType bitCount(keyType n)
{
    n -= (n >> 1) & 0x55555555;
    n = (n & 0x33333333) + ((n >> 2) & 0x33333333);
    n = (n & 0x0f0f0f0f) + ((n >> 4) & 0x0f0f0f0f);
    n += n >> 8;
    return (n + (n >> 16)) & 0x3f;
}

inline
indexType indexForBit(keyType const bitmap, keyType const bit)
{
    return bitCount(bitmap & (bit - 1));
}

inline
keyType maskBit(keyType const n, indexType const shift)
{
    return 1 << mask(n, shift);
}


// --------------------------------------------------------------------
// Array copying with small modifications
// --------------------------------------------------------------------

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

}
}

#endif // !ODF_HASH_TRIE_HPP
