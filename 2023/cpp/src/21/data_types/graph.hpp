//
// Created by Conor Pappas on 1/16/24.
//

#pragma once

#include <grid.hpp>
#include <graph/adjacency_list.hpp>
#include <graph/graph_traits.hpp>

#include "tile.hpp"

namespace step::data_types {
    using Graph = support::graph::AdjacencyList<Tile>;
    using graph_traits = support::graph::graph_traits<Graph>;
    using vertex_descriptor = graph_traits::vertex_descriptor;
    using Grid = support::Grid<Tile>;
    using coordinate = size_t;
    using Point = support::Point<coordinate, 2>;

    Graph make_graph(const Grid& grid);
    vertex_descriptor get_vertex(const Grid&, const Point&);
}
