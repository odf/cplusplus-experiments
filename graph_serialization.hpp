#ifndef ODF_GRAPH_SERIALIZATION_HPP
#define ODF_GRAPH_SERIALIZATION_HPP 1

#include <boost/serialization/split_free.hpp>


namespace boost { namespace serialization {

template<typename G, class Archive>
void save(Archive& ar, const G& graph, const unsigned int version)
{
    const int n = graph.nrVertices();
    ar & n;

    for (odf::List<typename G::vertex_type> p = graph.vertices();
         not p.isEmpty();
         p = p.rest())
    {
        const typename G::vertex_type v = p.first();
        const int m = graph.nrSuccessors(v);

        ar & v;
        ar & m;

        for (odf::List<typename G::vertex_type> q = graph.successors(v);
             not q.isEmpty();
             q = q.rest())
        {
            const typename G::vertex_type w = q.first();
            ar & w;
        }
    }
}

template<typename G, class Archive>
void load(Archive& ar, G& graph, const unsigned int version)
{
    int n;
    ar & n;

    for (int i = 0; i < n; ++i)
    {
        typename G::vertex_type v;
        int m;
        ar & v;
        ar & m;

        graph.addVertex(v);
        for (int j = 0; j < m; ++j)
        {
            typename G::vertex_type w;
            ar & w;
            graph.addEdge(v, w);
        }
    }
}

template<typename T, class Archive>
inline void serialize(Archive& ar,
                      odf::Graph<T>& graph,
                      const unsigned int file_version)
{
    split_free(ar, graph, file_version); 
}

}}

#endif // !ODF_GRAPH_SERIALIZATION_HPP
