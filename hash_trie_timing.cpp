/* -*-c++-*- */

#include "hash_trie.hpp"

using namespace odf::hash_trie;

hashType hashfun(int const val)
{
    return val;
}

typedef PersistentMap<int, int, hashfun> Map;

int main(int argc, char** argv)
{
    Map map;
    int const N = atoi(argv[1]);

    for (int i = 0; i < N; ++i)
        map = map.insert(i, i);

    Map copy = map;
    for (int i = 0; i < N; ++i)
        copy = copy.remove(i);
}
