//
// Created by Conor Pappas on 1/14/24.
//

#pragma once

#include <unordered_map>

#include <point.hpp>
#include <graph/adjacency_list.hpp>

#include "types.hpp"

namespace crucible::data_types {
    class HeatGraph : public support::graph::AdjacencyList<Vertex, Edge> {
        struct VertexPair {
            vertex_descriptor horizontal {};
            vertex_descriptor vertical {};
        };
        using Base = adjacency_list;
    public:
        explicit HeatGraph(Grid grid, coordinate min_distance, coordinate max_distance);
        [[nodiscard]] vertex_descriptor get_vertex(const Point& position, Direction) const;
        [[nodiscard]] vertex_descriptor get_vertex(const Vertex&) const;
        [[nodiscard]] const Grid& get_grid() const;
    private:
        coordinate m_min_distance {};
        coordinate m_max_distance {};
        Grid m_grid {};
        std::unordered_map<Point, VertexPair, Point::Hasher> m_vertex_map {};

        struct Private;
    };
}
