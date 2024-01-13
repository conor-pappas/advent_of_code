//
// Created by Conor Pappas on 1/7/24.
//

#pragma once

#include <list>
#include <ranges>

namespace support::graph {
    namespace impl {
        template<typename Vertex, typename ArcData>
        struct Arc {
            Arc(Vertex source, Vertex target, ArcData data = {}):
                m_source(source), m_target(target), m_data(data) {}
            Vertex m_source {};
            Vertex m_target {};
            ArcData m_data {};
        };

        template<typename AdjacencyContainer, typename VertexData>
        struct Vertex {
            explicit Vertex(VertexData data = {}): m_data(data) {}
            AdjacencyContainer m_out_arcs {};
            VertexData m_data {};
        };

        struct not_implemented {};
        using not_implemented_iterator = std::vector<not_implemented>::iterator;

        template <typename VertexData, std::default_initializable ArcData>
        struct types {
            using vertex_data = VertexData;
            using arc_data = ArcData;

            using vertex_descriptor = size_t;
            using Arc = Arc<vertex_descriptor, ArcData>;
            using arc_descriptor = const Arc&;
            using OutArcContainer = std::list<Arc>;
            using StoredVertex = Vertex<OutArcContainer, VertexData>;
            using VertexSetContainer = std::vector<StoredVertex>;

            using vertex_set_iterator = typename VertexSetContainer::iterator;
            using arc_set_iterator = not_implemented_iterator;
            using out_arc_iterator = typename OutArcContainer::const_iterator;
            using in_arc_iterator = not_implemented_iterator;
            using out_adjacency_iterator = not_implemented_iterator;
            using in_adjacency_iterator = not_implemented_iterator;

            using vertices_size_type = typename VertexSetContainer::size_type;
            using arcs_size_type = not_implemented;
            using degree_size_type = not_implemented;
        };

        template <typename Types>
        class adjacency_list {
        public:
            typename Types::VertexSetContainer vertices;
        };

        template <typename VertexData, typename ArcData>
        struct adjacency_list_gen {
            using type = adjacency_list<types<VertexData, ArcData>>;
        };
    }

    //==================================================================================================
    // Vertex Data
    //==================================================================================================

    template<typename Types>
    typename Types::VertexSetContainer vertices(const impl::adjacency_list<Types>& graph) {
        return graph.vertices;
    }

    template<typename Types>
    typename Types::vertices_size_type num_vertices(const impl::adjacency_list<Types>& graph) {
        return graph.vertices.size();
    }

    template<typename Types>
    typename Types::vertex_data vertex_data(
        const impl::adjacency_list<Types>& graph,
        typename Types::vertex_descriptor vertex_position)
    {
        return graph.vertices[vertex_position].m_data;
    }

    //==================================================================================================
    // Arc Data
    //==================================================================================================

    template<typename Types>
    std::ranges::subrange<typename Types::arc_set_iterator> arcs(const impl::adjacency_list<Types>&) {
        throw std::runtime_error("Not Implemented");
    }

    template<typename Types>
    typename Types::arcs_size_type num_arcs(const impl::adjacency_list<Types>&) {
        throw std::runtime_error("Not Implemented");
    }

    template<typename Types>
    typename Types::arc_data arc_data(
        const impl::adjacency_list<Types>&,
        const typename Types::arc_descriptor& arc)
    {
        return arc.m_data;
    }

    template<typename Types>
    typename Types::vertex_descriptor source(
        const impl::adjacency_list<Types>&,
        const typename Types::arc_descriptor& arc)
    {
        return arc.m_source;
    }

    template<typename Types>
    typename Types::vertex_descriptor target(
        const impl::adjacency_list<Types>&,
        const typename Types::arc_descriptor& arc)
    {
        return arc.m_target;
    }

    //==================================================================================================
    // Adjacency Data
    //==================================================================================================

    template<typename Types>
    const typename Types::OutArcContainer& out_arcs(
        const impl::adjacency_list<Types>& graph,
        typename Types::vertex_descriptor vertex)
    {
        return graph.vertices[vertex].m_out_arcs;
    }

    template<typename Types>
    std::ranges::subrange<typename Types::in_arc_iterator> in_arcs(
        const impl::adjacency_list<Types>&, typename Types::vertex_descriptor)
    {
        throw std::runtime_error("Not Implemented");
    }

    template<typename Types>
    std::ranges::subrange<typename Types::out_adjacency_iterator> out_adjacencies(
        const impl::adjacency_list<Types>&, typename Types::vertex_descriptor)
    {
        throw std::runtime_error("Not Implemented");
    }

    template<typename Types>
    std::ranges::subrange<typename Types::in_adjacency_iterator> in_adjacencies(
        const impl::adjacency_list<Types>&, typename Types::vertex_descriptor)
    {
        throw std::runtime_error("Not Implemented");
    }

    template<typename Types>
    typename Types::degree_size_type out_degree(
        const impl::adjacency_list<Types>& graph, typename Types::vertex_descriptor vertex)
    {
        return graph.vertices[vertex].m_out_arcs.size();
    }

    template<typename Types>
    typename Types::degree_size_type in_degree(
        const impl::adjacency_list<Types>&, typename Types::vertex_descriptor) {
        throw std::runtime_error("Not Implemented");
    }

    //==================================================================================================
    // Vertex Management
    //==================================================================================================

    template<typename Types>
    typename Types::vertex_descriptor add_vertex(impl::adjacency_list<Types>& graph) {
        return add_vertex(graph, typename Types::vertex_data{});
    }

    template<typename Types>
    typename Types::vertex_descriptor add_vertex(impl::adjacency_list<Types>& graph, typename Types::vertex_data data) {
        graph.vertices.emplace_back(data);
        return graph.vertices.size() - 1;
    }

    template<typename Types>
    void remove_vertex(impl::adjacency_list<Types>& graph, typename Types::vertex_descriptor vertex_position) {
        graph.vertices.erase(graph.vertices.begin() + vertex_position);
        throw std::runtime_error("Not Implemented"); // TODO: also break any existing arcs
    }

    //==================================================================================================
    // Arc Management
    //==================================================================================================

    template<typename Types>
    typename Types::arc_descriptor add_arc(
        impl::adjacency_list<Types>& graph,
        typename Types::vertex_descriptor u, typename Types::vertex_descriptor v)
    {
        return add_arc(graph, u, v, {});
    }

    template<typename Types>
    typename Types::arc_descriptor add_arc(
        impl::adjacency_list<Types>& graph,
        typename Types::vertex_descriptor source, typename Types::vertex_descriptor target,
        typename Types::arc_data data)
    {
        return graph.vertices[source].m_out_arcs.emplace_back(source, target, data);
    }

    template<typename Types>
    void remove_arc(impl::adjacency_list<Types>&, const typename Types::arc_descriptor&) {
        throw std::runtime_error("Not Implemented");
    }

    template<typename Types>
    void remove_arc(impl::adjacency_list<Types>&, typename Types::vertex_descriptor, typename Types::vertex_descriptor) {
        throw std::runtime_error("Not Implemented");
    }

}
