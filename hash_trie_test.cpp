/* -*-c++-*- */

#include <cassert>
#include <iostream>

#include "hash_trie.hpp"

using namespace odf::hash_trie;

hashType id(int const val)
{
    return val;
}


int main()
{
    assert(masked(0x12345678,  0) == 0x18);
    assert(masked(0x12345678,  5) == 0x13);
    assert(masked(0x12345678, 10) == 0x15);
    assert(masked(0x12345678, 15) == 0x08);
    assert(masked(0x12345678, 20) == 0x03);

    assert(bitCount(0x12345678) == 13);
    assert(bitCount(0xffffffff) == 32);
    assert(bitCount(0x80000000) ==  1);
    assert(bitCount(0x00000000) ==  0);
    assert(bitCount(0x00100400) ==  2);

    assert(indexForBit(0x01010101, 0x01000000) == 3);
    assert(indexForBit(0x01010101, 0x00010000) == 2);
    assert(indexForBit(0xffffffff, 0x00040000) == 18);
    assert(indexForBit(0xfffbffff, 0x00040000) == 18);

    int a[] = { 0, 1, 2, 3 };
    for (int k = 0; k < 4; ++k)
    {
        const int* b = arrayWith(a, 4, k, 5);
        const int* c = arrayWithInsertion(a, 4, k, 5);
        const int* d = arrayWithout(a, 4, k);

        for (int i = 0; i < 4; ++i)
        {
            assert(a[i] == i);
            assert(b[i] == ((i == k) ? 5 : i));
        }

        for (int i = 0; i < 5; ++i)
        {
            assert(c[i] == ((i < k) ? i : (i == k) ? 5 : i - 1));
        }

        for (int i = 0; i < 3; ++i)
        {
            assert(d[i] == ((i < k) ? i : i + 1));
        }
    }

    const int* x = arrayWithInsertion((int *) 0, 0, 0, 1);
    assert(x[0] == 1);

    typedef PersistentMap<int, int, id> Map;

    std::cout << "Empty map: " << std::endl << "  " << Map() << std::endl;

    Map map = Map().insert(1, 2).insert(3, 4).insert(259, 5);
    std::cout << "More interesting one: " << std::endl
              << "  " << map << std::endl;

    assert(*map.get(1) == 2);
    assert(*map.get(3) == 4);
    assert(*map.get(259) == 5);

    std::cout << "Everything seems okay." << std::endl;
}
