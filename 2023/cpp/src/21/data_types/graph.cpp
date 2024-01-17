//
// Created by Conor Pappas on 1/16/24.
//

#include "graph.hpp"

#include <cassert>

namespace step::data_types {
    vertex_descriptor get_vertex(const Grid& grid, const Point& point) {
        return point.y() * grid.width() + point.x();
    }

    Graph make_graph(const Grid& grid) {
        Graph graph;
        const auto width = grid.width();
        const auto height = grid.height();

        // TODO: Add pre-allocation for graphs
        for(size_t i=0; i<height*width; ++i) add_vertex(graph);

        for(size_t y=0; y<height; ++y) {
            for(size_t x=0; x<width; ++x) {
                Point position {x, y};
                std::array<Point,4> neighbors = {
                    Point {x, y+1},
                    Point {x, y-1},
                    Point {x+1, y},
                    Point {x-1, y}
                };
                for(const auto neighbor: neighbors) {
                    if (grid.contains(neighbor) && grid.get(neighbor) != Tile::Rock) {
                        add_arc(graph, get_vertex(grid, position), get_vertex(grid, neighbor));
                    }
                }
            }
        }
        return graph;
    }

}
