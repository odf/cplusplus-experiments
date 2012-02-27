/* -*-c++-*- */

#include <cassert>

#include "hash_trie.hpp"

using namespace odf::hash_trie;

int main()
{
    assert(mask(0x12345678,  0) == 0x18);
    assert(mask(0x12345678,  5) == 0x13);
    assert(mask(0x12345678, 10) == 0x15);
    assert(mask(0x12345678, 15) == 0x08);
    assert(mask(0x12345678, 20) == 0x03);

    assert(bitCount(0x12345678) == 13);
    assert(bitCount(0xffffffff) == 32);
    assert(bitCount(0x80000000) ==  1);
    assert(bitCount(0x00000000) ==  0);
    assert(bitCount(0x00100400) ==  2);

    assert(indexForBit(0x01010101, 0x01000000) == 3);
    assert(indexForBit(0x01010101, 0x00010000) == 2);
    assert(indexForBit(0xffffffff, 0x00040000) == 18);
}
