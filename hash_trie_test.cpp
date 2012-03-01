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
#define CHECK_MISSING(key, map) CHECK_EQUAL(None, (map).get(key).get())

    const void* None = 0;

    TEST(SimpleTest)
    {
        Map map = Map().insert(1, 2).insert(3, 4).insert(259, 5).insert(1027, 6);

        CHECK_EQUAL(2, *map.get(   1));
        CHECK_EQUAL(4, *map.get(   3));
        CHECK_EQUAL(5, *map.get( 259));
        CHECK_EQUAL(6, *map.get(1027));

        CHECK_MISSING(123, map);
    }

    TEST(TwoInOneOut)
    {
        Map map = Map().insert('A', 1).insert('B', 2).remove('A');

        CHECK_EQUAL(1, map.size());
        CHECK_EQUAL(2, *map.get('B'));
        CHECK_MISSING('A', map);
    }

    TEST(EmptyMap)
    {
        Map map;

        CHECK_EQUAL(0, map.size());
        CHECK_MISSING(0, map);
        CHECK_EQUAL(0, map.remove(0).size());
        CHECK_EQUAL("PersistentMap({})", map.asString());
    }

    TEST(SingletonMap)
    {
        Map map = Map().insert('A', 1);

        CHECK_EQUAL(1, map.size());
        CHECK_EQUAL(1, *map.get('A'));
        CHECK_MISSING('B', map);
        CHECK_EQUAL(0, map.remove('A').remove('A').size());
        CHECK_EQUAL("PersistentMap(65 -> 1)", map.asString());
    }

    TEST(SingletonMapUpdate)
    {
        Map map = Map().insert('A', 1).insert('A', 65);

        CHECK_EQUAL(1, map.size());
        CHECK_EQUAL(65, *map.get('A'));
        CHECK_MISSING('B', map);
    }

    SUITE(TwoItemsLevelOneCollision)
    {
        int const key_a = 1;
        int const key_b = 33;
        int const key_c = 5;

        Map map = Map().insert(key_a, 'a').insert(key_b, 'b');

        TEST(Basic)
        {
            CHECK_EQUAL(2, map.size());
            CHECK_EQUAL('a', *map.get(key_a));
            CHECK_EQUAL('a', *map.get(key_a));
            CHECK_MISSING(key_c, map);
            CHECK_EQUAL("PersistentMap({1: {0: 1 -> 97, 1: 33 -> 98}})",
                        map.asString());

            CHECK_EQUAL(1, map.remove(key_a).size());
            CHECK_EQUAL(0, map.remove(key_a).remove(key_b).size());
        }

        TEST(RemoveNonMember)
        {
            Map mod = map.remove(key_c);

            // CHECK_EQUAL(2, mod.size());
            // CHECK_EQUAL('a', *mod.get(key_a));
            // CHECK_EQUAL('a', *mod.get(key_a));
            // CHECK_MISSING(key_c, mod);
        }
    }
}

int main()
{
    return UnitTest::RunAllTests();
}
