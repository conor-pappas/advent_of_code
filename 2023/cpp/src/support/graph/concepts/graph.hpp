//
// Created by Conor Pappas on 1/11/24.
//

#pragma once

#include "range_concepts.hpp"

namespace support::graph::concepts {

    template<typename I>
    using subrange = std::ranges::subrange<I>;

    template<typename G>
    concept GraphTypes = requires(G&& g)
    {
        typename G::vertex_descriptor;
        typename G::arc_descriptor;
        typename G::vertex_data;
        typename G::arc_data;

        typename G::vertex_set_iterator;
        typename G::arc_set_iterator;
        typename G::out_arc_iterator;
        typename G::in_arc_iterator;
        typename G::out_adjacency_iterator;
        typename G::in_adjacency_iterator;

        typename G::vertices_size_type;
        typename G::arcs_size_type;
        typename G::degree_size_type;
    };

    template<typename G>
    concept GraphConstructors =
        std::default_initializable<G> &&
        std::copy_constructible<G>;

    // TODO: This seems like a generic concept for "set management" of a given type
    template<typename G>
    concept VertexData = requires(const G&& g, typename G::vertex_descriptor&& v)
    {
        { vertices(g) } -> range_iterated_by<typename G::vertex_set_iterator>;
        { num_vertices(g) } -> std::same_as<typename G::vertices_size_type>;
        { vertex_data(g, v) } -> std::same_as<typename G::vertex_data>;
    };

    template<typename G>
    concept ArcData = requires(const G&& g, const typename G::arc_descriptor&& a)
    {
        { arcs(g) } -> range_iterated_by<typename G::arc_set_iterator>;
        { num_arcs(g) } -> std::same_as<typename G::arcs_size_type>;
        { arc_data(g, a) } -> std::same_as<typename G::arc_data>;
        { source(g, a) } -> std::same_as<typename G::vertex_descriptor>;
        { target(g, a) } -> std::same_as<typename G::vertex_descriptor>;
    };

    template<typename G>
    concept AdjacencyData = requires(const G&& g, typename G::vertex_descriptor&& v)
    {
        { out_arcs(g, v) } -> range_iterated_by<typename G::out_arc_iterator>;
        { in_arcs(g, v) } -> range_iterated_by<typename G::in_arc_iterator>;
        { out_adjacencies(g, v) } -> range_iterated_by<typename G::out_adjacency_iterator>;
        { in_adjacencies(g, v) } -> range_iterated_by<typename G::in_adjacency_iterator>;
        { out_degree(g,v) } -> std::same_as<typename G::degree_size_type>;
        { in_degree(g,v) } -> std::same_as<typename G::degree_size_type>;
    };

    template<typename G>
    concept VertexModification = requires(G&& g, typename G::vertex_descriptor v, typename G::vertex_data&& data)
    {
        { add_vertex(g) } -> std::same_as<decltype(v)>;
        { add_vertex(g, data) } -> std::same_as<decltype(v)>;
        { remove_vertex(g, v) } -> std::same_as<void>;
    };

    template<typename G>
    concept ArcModification = requires(
        G&& g,
        typename G::arc_descriptor&& a,
        typename G::vertex_descriptor u,
        typename G::vertex_descriptor v,
        typename G::arc_data&& data)
    {
        { add_arc(g, u, v) } -> std::same_as<decltype(a)>;
        { add_arc(g, u, v, data) } -> std::same_as<decltype(a)>;
        { remove_arc(g, a) } -> std::same_as<void>;
        { remove_arc(g, u, v) } -> std::same_as<void>;
    };

    template<typename G>
    concept GraphData = VertexData<G> && ArcData<G> && AdjacencyData<G>;

    template<typename G>
    concept GraphModification = VertexModification<G> && ArcModification<G>;

    template<typename G>
    concept Graph = GraphTypes<G> && GraphConstructors<G> && GraphData<G> && GraphModification<G>;

}
