//
// Created by Conor Pappas on 1/7/24.
//

#pragma once

#include "impl/adjacenty_list.hpp"

namespace support::graph {
    struct no_data {};

    template <std::default_initializable VertexData = no_data, std::default_initializable ArcData = no_data>
    class AdjacencyList: public impl::adjacency_list_gen<VertexData, ArcData>::type {
        using Base = typename impl::adjacency_list_gen<VertexData, ArcData>::type;
        using types = impl::types<VertexData, ArcData>;
    public:
        using vertex_descriptor = typename types::vertex_descriptor;
        using arc_descriptor = typename types::arc_descriptor;
        using vertex_data = VertexData;
        using arc_data = ArcData;

        using vertex_set_iterator = typename types::vertex_set_iterator;
        using arc_set_iterator = typename types::arc_set_iterator;
        using out_arc_iterator = typename types::out_arc_iterator;
        using in_arc_iterator = typename types::in_arc_iterator;
        using out_adjacency_iterator = typename types::out_adjacency_iterator;
        using in_adjacency_iterator = typename types::in_adjacency_iterator;

        using vertices_size_type = typename types::vertices_size_type;
        using arcs_size_type = typename types::arcs_size_type;
        using degree_size_type = typename types::degree_size_type;
    };

}
