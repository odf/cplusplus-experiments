/* -*-c++-*- */

#include <unittest++/UnitTest++.h>

#include <cassert>

#include "hash_trie.hpp"

using namespace odf::hash_trie;

hashType hashfun(int const val)
{
    return val % 256;
}

typedef PersistentMap<int, int, hashfun> Map;


SUITE(BitTwiddling)
{
    TEST(MaskedFunction)
    {
        CHECK_EQUAL(0x18, masked(0x12345678,  0));
        CHECK_EQUAL(0x13, masked(0x12345678,  5));
        CHECK_EQUAL(0x15, masked(0x12345678, 10));
        CHECK_EQUAL(0x08, masked(0x12345678, 15));
        CHECK_EQUAL(0x03, masked(0x12345678, 20));
    }

    TEST(BitCountFunction)
    {
        CHECK_EQUAL(13, bitCount(0x12345678));
        CHECK_EQUAL(32, bitCount(0xffffffff));
        CHECK_EQUAL( 1, bitCount(0x80000000));
        CHECK_EQUAL( 0, bitCount(0x00000000));
        CHECK_EQUAL( 2, bitCount(0x00100400));
    }

    TEST(IndexForBitFunction)
    {
        CHECK_EQUAL( 3, indexForBit(0x01010101, 0x01000000));
        CHECK_EQUAL( 2, indexForBit(0x01010101, 0x00010000));
        CHECK_EQUAL(18, indexForBit(0xffffffff, 0x00040000));
        CHECK_EQUAL(18, indexForBit(0xfffbffff, 0x00040000));
    }
}

SUITE(ArrayCopyOnWrite)
{
    int in[] = { 0, 1, 2, 3 };

    TEST(ArrayWithFunction)
    {
        for (int k = 0; k < 4; ++k)
        {
            const int* out = arrayWith(in, 4, k, 5);
            
            for (int i = 0; i < 4; ++i)
                CHECK_EQUAL(i, in[i]);

            for (int i = 0; i < 4; ++i)
                CHECK_EQUAL(((i == k) ? 5 : i), out[i]);
        }
    }

    TEST(ArrayWithInsertionFunction)
    {
        for (int k = 0; k < 4; ++k)
        {
            const int* out = arrayWithInsertion(in, 4, k, 5);
            
            for (int i = 0; i < 4; ++i)
                CHECK_EQUAL(i, in[i]);
            
            for (int i = 0; i < 5; ++i)
                CHECK_EQUAL(((i < k) ? i : (i == k) ? 5 : i - 1), out[i]);
        }
    }

    TEST(ArrayWithoutFunction)
    {
        for (int k = 0; k < 4; ++k)
        {
            const int* out = arrayWithout(in, 4, k);
            
            for (int i = 0; i < 4; ++i)
                CHECK_EQUAL(i, in[i]);
            
            for (int i = 0; i < 3; ++i)
                CHECK_EQUAL(((i < k) ? i : i + 1), out[i]);
        }
    }

    TEST(EmptyArray)
    {
        const int* x = arrayWithInsertion((int *) 0, 0, 0, 1);
        CHECK_EQUAL(1, x[0]);
    }
}

SUITE(PersistentMap)
{
    const void* None = 0;

    TEST(EmptyMap)
    {
        Map map;

        CHECK_EQUAL(0, map.size());
        CHECK_EQUAL(None, map.get(0).get());
        CHECK_EQUAL(0, map.remove(0).size());
        CHECK_EQUAL("PersistentMap(EmptyNode)", map.asString());
    }

    TEST(SimpleTest)
    {
        Map map = Map().insert(1, 2).insert(3, 4).insert(259, 5).insert(1027, 6);

        CHECK_EQUAL(2, *map.get(   1));
        CHECK_EQUAL(4, *map.get(   3));
        CHECK_EQUAL(5, *map.get( 259));
        CHECK_EQUAL(6, *map.get(1027));

        CHECK_EQUAL(None, map.get( 123).get());
    }

    TEST(TwoInOneOut)
    {
        Map map = Map().insert('A', 1).insert('B', 2).remove('A');

        CHECK_EQUAL(1, map.size());
        CHECK_EQUAL(2, *map.get('B'));
        CHECK_EQUAL(None, map.get('A').get());
    }
}

int main()
{
    return UnitTest::RunAllTests();
}
