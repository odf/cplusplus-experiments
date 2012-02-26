/* -*-c++-*- */

#ifndef ODF_HASH_TRIE_HPP
#define ODF_HASH_TRIE_HPP 1

#include <stdint.h>

namespace odf
{
namespace hash_trie
{
typedef uint32_t      key_type;
typedef unsigned char index_type;

key_type mask(const key_type n, const index_type shift)
{
    return (n >> shift) & 0x1f;
}

index_type bitCount(key_type n)
{
    n -= (n >> 1) & 0x55555555;
    n = (n & 0x33333333) + ((n >> 2) & 0x33333333);
    n = (n & 0x0f0f0f0f) + ((n >> 4) & 0x0f0f0f0f);
    n += n >> 8;
    return (n + (n >> 16)) & 0x3f;
}

index_type indexForBit(const key_type bitmap, const key_type bit)
{
    return bitCount(bitmap & (bit - 1));
}

key_type maskBit(const key_type n, const index_type shift)
{
    return 1 << mask(n, shift);
}

}
}

#endif // !ODF_HASH_TRIE_HPP
