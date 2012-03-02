/* -*-c++-*- */

#include <stdlib.h>

#include "hash_trie.hpp"

using namespace odf::hash_trie;

hashType hashfun(int const val)
{
    return val;
}

typedef PersistentMap<int, int, hashfun> Map;

int main(int argc, char** argv)
{
    int const N = atoi(argv[1]);
    size_t* values = new size_t[N];

    srand(123456789);

    for (int i = 0; i < N; ++i)
    {
        values[i] = rand();
    }

    Map map;

    for (int i = 0; i < N; ++i)
        map = map.insert(values[i], i);

    double sum = 0.0;
    for (int i = N / 2; i < N; ++i)
    {
        sum += map.getVal(values[i], 0);
    }

    std::cout << "RAND_MAX = " << RAND_MAX << std::endl;
    std::cout << "sum = " << sum << std::endl;

    Map copy = map;
    for (int i = 0; i < N; i += 2)
        copy = copy.remove(values[i]);
}
