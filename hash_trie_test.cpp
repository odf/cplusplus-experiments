/* -*-c++-*- */

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE HashTrie
#include <boost/test/unit_test.hpp>

#include <cassert>
#include <iostream>

#include "hash_trie.hpp"

using namespace odf::hash_trie;

hashType hashfun(int const val)
{
    return val % 1024;
}


BOOST_AUTO_TEST_CASE( all_tests )
{
    BOOST_CHECK_EQUAL(masked(0x12345678,  0), 0x18);
    BOOST_CHECK_EQUAL(masked(0x12345678,  5), 0x13);
    BOOST_CHECK_EQUAL(masked(0x12345678, 10), 0x15);
    BOOST_CHECK_EQUAL(masked(0x12345678, 15), 0x08);
    BOOST_CHECK_EQUAL(masked(0x12345678, 20), 0x03);

    BOOST_CHECK_EQUAL(bitCount(0x12345678), 13);
    BOOST_CHECK_EQUAL(bitCount(0xffffffff), 32);
    BOOST_CHECK_EQUAL(bitCount(0x80000000),  1);
    BOOST_CHECK_EQUAL(bitCount(0x00000000),  0);
    BOOST_CHECK_EQUAL(bitCount(0x00100400),  2);

    BOOST_CHECK_EQUAL(indexForBit(0x01010101, 0x01000000), 3);
    BOOST_CHECK_EQUAL(indexForBit(0x01010101, 0x00010000), 2);
    BOOST_CHECK_EQUAL(indexForBit(0xffffffff, 0x00040000), 18);
    BOOST_CHECK_EQUAL(indexForBit(0xfffbffff, 0x00040000), 18);

    int a[] = { 0, 1, 2, 3 };
    for (int k = 0; k < 4; ++k)
    {
        const int* b = arrayWith(a, 4, k, 5);
        const int* c = arrayWithInsertion(a, 4, k, 5);
        const int* d = arrayWithout(a, 4, k);

        for (int i = 0; i < 4; ++i)
        {
            BOOST_CHECK_EQUAL(a[i], i);
            BOOST_CHECK_EQUAL(b[i], ((i == k) ? 5 : i));
        }

        for (int i = 0; i < 5; ++i)
        {
            BOOST_CHECK_EQUAL(c[i], ((i < k) ? i : (i == k) ? 5 : i - 1));
        }

        for (int i = 0; i < 3; ++i)
        {
            BOOST_CHECK_EQUAL(d[i], ((i < k) ? i : i + 1));
        }
    }

    const int* x = arrayWithInsertion((int *) 0, 0, 0, 1);
    BOOST_CHECK_EQUAL(x[0], 1);

    typedef PersistentMap<int, int, hashfun> Map;

    Map map = Map().insert(1, 2).insert(3, 4).insert(259, 5).insert(1027, 6);

    BOOST_CHECK_EQUAL(*map.get(   1), 2);
    BOOST_CHECK_EQUAL(*map.get(   3), 4);
    BOOST_CHECK_EQUAL(*map.get( 259), 5);
    BOOST_CHECK_EQUAL(*map.get(1027), 6);
}
