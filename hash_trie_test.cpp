/* -*-c++-*- */

#include <UnitTest++.h>

#include <cassert>

#include "hash_trie.hpp"

using namespace odf::hash_trie;

hashType hashfun(int const val)
{
    return val % 1024;
}


TEST(BitTwiddling)
{
    CHECK_EQUAL(masked(0x12345678,  0), 0x18);
    CHECK_EQUAL(masked(0x12345678,  5), 0x13);
    CHECK_EQUAL(masked(0x12345678, 10), 0x15);
    CHECK_EQUAL(masked(0x12345678, 15), 0x08);
    CHECK_EQUAL(masked(0x12345678, 20), 0x03);

    CHECK_EQUAL(bitCount(0x12345678), 13);
    CHECK_EQUAL(bitCount(0xffffffff), 32);
    CHECK_EQUAL(bitCount(0x80000000),  1);
    CHECK_EQUAL(bitCount(0x00000000),  0);
    CHECK_EQUAL(bitCount(0x00100400),  2);

    CHECK_EQUAL(indexForBit(0x01010101, 0x01000000), 3);
    CHECK_EQUAL(indexForBit(0x01010101, 0x00010000), 2);
    CHECK_EQUAL(indexForBit(0xffffffff, 0x00040000), 18);
    CHECK_EQUAL(indexForBit(0xfffbffff, 0x00040000), 18);
}

TEST(ArrayCopyOnWrite)
{
    int a[] = { 0, 1, 2, 3 };
    for (int k = 0; k < 4; ++k)
    {
        const int* b = arrayWith(a, 4, k, 5);
        const int* c = arrayWithInsertion(a, 4, k, 5);
        const int* d = arrayWithout(a, 4, k);

        for (int i = 0; i < 4; ++i)
        {
            CHECK_EQUAL(a[i], i);
            CHECK_EQUAL(b[i], ((i == k) ? 5 : i));
        }

        for (int i = 0; i < 5; ++i)
        {
            CHECK_EQUAL(c[i], ((i < k) ? i : (i == k) ? 5 : i - 1));
        }

        for (int i = 0; i < 3; ++i)
        {
            CHECK_EQUAL(d[i], ((i < k) ? i : i + 1));
        }
    }

    const int* x = arrayWithInsertion((int *) 0, 0, 0, 1);
    CHECK_EQUAL(x[0], 1);
}

TEST(PersistentMap)
{
    typedef PersistentMap<int, int, hashfun> Map;

    Map map = Map().insert(1, 2).insert(3, 4).insert(259, 5).insert(1027, 6);

    CHECK_EQUAL(*map.get(   1), 2);
    CHECK_EQUAL(*map.get(   3), 4);
    CHECK_EQUAL(*map.get( 259), 5);
    CHECK_EQUAL(*map.get(1027), 6);
}

int main()
{
    return UnitTest::RunAllTests();
}
