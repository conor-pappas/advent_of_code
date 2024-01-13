//
// Created by Conor Pappas on 1/13/24.
//

#pragma once

#include "graph/concepts/graph.hpp"

namespace support::graph {

    template <concepts::Graph Graph>
    struct graph_traits {
        using vertex_descriptor = typename Graph::vertex_descriptor;
        using arc_descriptor = typename Graph::arc_descriptor;
        using vertex_data = typename Graph::vertex_data;
        using arc_data = typename Graph::arc_data;

        using vertex_set_iterator = typename Graph::vertex_set_iterator;
        using arc_set_iterator = typename Graph::arc_set_iterator;
        using out_arc_iterator = typename Graph::out_arc_iterator;
        using in_arc_iterator = typename Graph::in_arc_iterator;
        using out_adjacency_iterator = typename Graph::out_adjacency_iterator;
        using in_adjacency_iterator = typename Graph::in_adjacency_iterator;

        using vertices_size_type = typename Graph::vertices_size_type;
        using arcs_size_type = typename Graph::arcs_size_type;
        using degree_size_type = typename Graph::degree_size_type;
    };

}
