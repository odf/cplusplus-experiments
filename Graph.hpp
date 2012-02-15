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
pair<T, T> makeEdge(const T v, const T w)
{
    return pair<T, T>(v, w);
}

template<typename T>
pair<T, T> makeReverseEdge(const T v, const T w)
{
    return pair<T, T>(w, v);
}


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

    explicit Graph(const List<edge_type>& edges) :
        verts_(), forw_(), back_()
    {
        for(List<edge_type> p = edges; not p.isEmpty(); p = p.rest())
        {
            T from, to;
            tie(from, to) = p.first();
            addEdge(from, to);
        }
    }

    Graph& addEdge(const T& from, const T& to)
    {
        addVertex(from);
        addVertex(to);
        forw_[from].insert(to);
        back_[to].insert(from);

        return *this;
    }

    Graph& addVertex(const T& v)
    {
        verts_.insert(v);
        if (forw_.count(v) == 0)
        {
            forw_[v] = vertex_collection_type();
        }
        if (back_.count(v) == 0)
        {
            back_[v] = vertex_collection_type();
        }

        return *this;
    }

    Graph& removeEdge(const T& from, const T& to)
    {
        if (forw_.count(from) > 0 and forw_.at(from).count(to) > 0)
        {
            forw_[from].erase(to);
            back_[to].erase(from);
        }

        return *this;
    }

    Graph& removeEdge(const edge_type& e)
    {
        removeEdge(e.first, e.second);

        return *this;
    }

    Graph& removeVertex(const T& v)
    {
        if (verts_.count(v) > 0)
        {
            for (List<edge_type> p = concat(edgesFrom(v), edgesTo(v));
                 not p.isEmpty();
                 p = p.rest())
            {
                removeEdge(p.first());
            }
            verts_.erase(v);
        }

        return *this;
    }

    int nrVertices() const
    {
        return verts_.size();
    }

    List<vertex_type> vertices() const
    {
        return asList(verts_);
    }

    int nrSuccessors(const T& v) const
    {
        return forw_.at(v).size();
    };

    List<vertex_type> successors(const T& v) const
    {
        return asList(forw_.at(v));
    }

    int nrPredecessors(const T& v) const
    {
        return back_.at(v).size();
    }

    List<vertex_type> predecessors(const T& v) const
    {
        return asList(back_.at(v));
    }

    List<edge_type> edgesFrom(const T& v) const
    {
        return mapList(successors(v), curry(makeEdge<T>, v));
    }

    List<edge_type> edgesTo(const T& v) const
    {
        return mapList(predecessors(v), curry(makeReverseEdge<T>, v));
    }

    int nrEdges() const
    {
        return sum(mapList(vertices(), curry(&Graph::nrSuccessors, *this)));
    }

    List<edge_type> edges() const
    {
        return flatMap(vertices(), curry(&Graph::edgesFrom, *this));
    }
};

}

#endif // !ODF_GRAPH_HPP
