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
        CHECK_EQUAL(masked(0x12345678,  0), 0x18);
        CHECK_EQUAL(masked(0x12345678,  5), 0x13);
        CHECK_EQUAL(masked(0x12345678, 10), 0x15);
        CHECK_EQUAL(masked(0x12345678, 15), 0x08);
        CHECK_EQUAL(masked(0x12345678, 20), 0x03);
    }

    TEST(BitCountFunction)
    {
        CHECK_EQUAL(bitCount(0x12345678), 13);
        CHECK_EQUAL(bitCount(0xffffffff), 32);
        CHECK_EQUAL(bitCount(0x80000000),  1);
        CHECK_EQUAL(bitCount(0x00000000),  0);
        CHECK_EQUAL(bitCount(0x00100400),  2);
    }

    TEST(IndexForBitFunction)
    {
        CHECK_EQUAL(indexForBit(0x01010101, 0x01000000), 3);
        CHECK_EQUAL(indexForBit(0x01010101, 0x00010000), 2);
        CHECK_EQUAL(indexForBit(0xffffffff, 0x00040000), 18);
        CHECK_EQUAL(indexForBit(0xfffbffff, 0x00040000), 18);
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
                CHECK_EQUAL(in[i], i);

            for (int i = 0; i < 4; ++i)
                CHECK_EQUAL(out[i], ((i == k) ? 5 : i));
        }
    }

    TEST(ArrayWithInsertionFunction)
    {
        for (int k = 0; k < 4; ++k)
        {
            const int* out = arrayWithInsertion(in, 4, k, 5);
            
            for (int i = 0; i < 4; ++i)
                CHECK_EQUAL(in[i], i);
            
            for (int i = 0; i < 5; ++i)
                CHECK_EQUAL(out[i], ((i < k) ? i : (i == k) ? 5 : i - 1));
        }
    }

    TEST(ArrayWithoutFunction)
    {
        for (int k = 0; k < 4; ++k)
        {
            const int* out = arrayWithout(in, 4, k);
            
            for (int i = 0; i < 4; ++i)
                CHECK_EQUAL(in[i], i);
            
            for (int i = 0; i < 3; ++i)
                CHECK_EQUAL(out[i], ((i < k) ? i : i + 1));
        }
    }

    TEST(EmptyArray)
    {
        const int* x = arrayWithInsertion((int *) 0, 0, 0, 1);
        CHECK_EQUAL(x[0], 1);
    }
}

SUITE(PersistentMap)
{
    TEST(SimpleTest)
    {
        Map map = Map().insert(1, 2).insert(3, 4).insert(259, 5).insert(1027, 6);

        CHECK_EQUAL(*map.get(   1), 2);
        CHECK_EQUAL(*map.get(   3), 4);
        CHECK_EQUAL(*map.get( 259), 5);
        CHECK_EQUAL(*map.get(1027), 6);

        CHECK_EQUAL(map.get( 123).get(), (void *) 0);
    }
}

int main()
{
    return UnitTest::RunAllTests();
}
