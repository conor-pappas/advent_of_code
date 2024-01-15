//
// Created by Conor Pappas on 1/14/24.
//

#include "heat_graph.hpp"

#include <utility>

namespace crucible::data_types {

    struct HeatGraph::Private {
        static void init_heat_graph(HeatGraph&);
        static VertexPair create_vertices(HeatGraph&, const Point&);

        static void create_arcs(HeatGraph&heat_graph, const Point&, Direction, coordinate scale);
        static void create_arcs(HeatGraph&heat_graph, const Vertex&vertex, coordinate scale);
        static vertex_descriptor get_vertex_descriptor(HeatGraph&, const Point&, Direction);
        static Point arc_direction(const Vertex& vertex);
    };

    HeatGraph::HeatGraph(Grid  grid, const coordinate min_distance, const coordinate max_distance):
        m_min_distance(min_distance),
        m_max_distance(max_distance),
        m_grid(std::move(grid)) {
        Private::init_heat_graph(*this);
    }

    HeatGraph::vertex_descriptor HeatGraph::get_vertex(const Point& position, const Direction direction) const {
        const auto [horizontal, vertical] = m_vertex_map.at(position);
        return (direction == Direction::Horizontal) ? horizontal : vertical;
    }

    support::graph::adjacency_list<Vertex, Edge>::vertex_descriptor HeatGraph::get_vertex(const Vertex& vertex) const {
        return get_vertex(vertex.position, vertex.arrived_from);
    }

    const Grid& HeatGraph::get_grid() const {
        return m_grid;
    }

    void HeatGraph::Private::init_heat_graph(HeatGraph& heat_graph) {
        const auto& grid = heat_graph.m_grid;
        auto& vertex_map = heat_graph.m_vertex_map;

        for (coordinate y = 0; y < grid.height(); ++y) {
            for (coordinate x = 0; x < grid.width(); ++x) {
                auto position = Point { x, y };
                vertex_map[position] = create_vertices(heat_graph, position);
            }
        }

        for (coordinate y = 0; y < grid.height(); ++y) {
            for (coordinate x = 0; x < grid.width(); ++x) {
                auto position = Point { x, y };
                create_arcs(heat_graph, position, Direction::Horizontal, 1);
                create_arcs(heat_graph, position, Direction::Horizontal, -1);
                create_arcs(heat_graph, position, Direction::Vertical, 1);
                create_arcs(heat_graph, position, Direction::Vertical, -1);
            }
        }
    }

    HeatGraph::VertexPair HeatGraph::Private::create_vertices(HeatGraph& heat_graph, const Point& position) {
        const auto heat_lost = heat_graph.m_grid.get(position);
        const Vertex h_vertex { position, heat_lost, Direction::Horizontal };
        const Vertex v_vertex { position, heat_lost, Direction::Vertical };
        return {
            add_vertex(heat_graph, h_vertex),
            add_vertex(heat_graph, v_vertex)
        };
    }

    void HeatGraph::Private::create_arcs(
        HeatGraph& heat_graph, const Point& point, const Direction direction, const coordinate scale) {
        const auto descriptor = get_vertex_descriptor(heat_graph, point, direction);
        const auto vertex = support::graph::vertex_data(heat_graph, descriptor);
        create_arcs(heat_graph, vertex, scale);
    }

    void HeatGraph::Private::create_arcs(
        HeatGraph& heat_graph, const Vertex& vertex, const coordinate scale)
    {
        const auto unit = scale * arc_direction(vertex);
        const Point start = vertex.position;
        Point end = start + unit;
        heat total_cost = 0;
        const auto start_vertex = get_vertex_descriptor(heat_graph, start, vertex.arrived_from);
        for(coordinate i = 1; i <= heat_graph.m_max_distance && heat_graph.m_grid.contains(end); ++i, end += unit) {
            const auto end_vertex = get_vertex_descriptor(heat_graph, end, opposite(vertex.arrived_from));
            total_cost += support::graph::vertex_data(heat_graph, end_vertex).heat_loss;
            if(i >= heat_graph.m_min_distance) {
                add_arc(heat_graph, start_vertex, end_vertex, {total_cost});
            }
        }
    }

     HeatGraph::vertex_descriptor HeatGraph::Private::get_vertex_descriptor(
         HeatGraph& heat_graph, const Point& point, const Direction direction)
    {
        const auto [horizontal, vertical] = heat_graph.m_vertex_map.at(point);
        return (direction == Direction::Horizontal) ? horizontal : vertical;
    }

     Point HeatGraph::Private::arc_direction(const Vertex& vertex) {
        return vertex.arrived_from == Direction::Horizontal ? Point::unit(0) : Point::unit(1);
     }
}
