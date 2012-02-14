#ifndef ODF_GRAPH_HPP
#define ODF_GRAPH_HPP 1

#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>
#include <boost/tuple/tuple.hpp>

#include "List.hpp"
#include "list_fun.hpp"


namespace odf
{

using boost::unordered_set;
using boost::unordered_map;
using std::pair;
using boost::tie;

template<typename T>
class Graph
{
private:
    unordered_set<T>                    verts_;
    unordered_map<T, unordered_set<T> > forw_;
    unordered_map<T, unordered_set<T> > back_;

public:
    typedef T          vertex_type;
    typedef pair<T, T> edge_type;

    template<class Iterator>
    Graph(Iterator iter, const Iterator end) :
        verts_(), forw_(), back_()
    {
        for(; iter != end; ++iter)
        {
            T from, to;
            tie(from, to) = *iter;
            addEdge(from, to);
        }
    }

    void addEdge(const T from, const T to)
    {
        addVertex(from);
        addVertex(to);
        forw_[from].insert(to);
        back_[to].insert(from);
    }

    void addVertex(const T v)
    {
        verts_.insert(v);
    }

    const int nvertices() const
    {
        return verts_.size();
    }

    const List<vertex_type> vertices() const
    {
        return asList(verts_);
    }
};

}

#endif // !ODF_GRAPH_HPP
