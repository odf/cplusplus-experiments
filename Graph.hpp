#ifndef ODF_GRAPH_HPP
#define ODF_GRAPH_HPP 1

#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>
#include <boost/tuple/tuple.hpp>

#include "List.hpp"
#include "fun.hpp"
#include "list_fun.hpp"


namespace odf
{

using std::pair;
using boost::unordered_set;
using boost::unordered_map;
using boost::tie;

template<typename T>
class Graph
{
private:
    typedef unordered_set<T>                          vertex_collection_type;
    typedef unordered_map<T, vertex_collection_type > adj_lists_map_type;
    typedef pair<T, vertex_collection_type >          adj_lists_item_type;

    vertex_collection_type verts_;
    adj_lists_map_type     forw_;
    adj_lists_map_type     back_;

public:
    typedef T          vertex_type;
    typedef pair<T, T> edge_type;

    template<class Iterator>
    explicit Graph(Iterator iter, const Iterator end) :
        verts_(), forw_(), back_()
    {
        for(; iter != end; ++iter)
        {
            T from, to;
            tie(from, to) = *iter;
            addEdge(from, to);
        }
    }

    explicit Graph(const List<edge_type> edges) :
        verts_(), forw_(), back_()
    {
        for(List<edge_type> p = edges; not p.isEmpty(); p = p.rest())
        {
            T from, to;
            tie(from, to) = p.first();
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

    const int nedges() const
    {
        return 0;
    }
};

}

#endif // !ODF_GRAPH_HPP
