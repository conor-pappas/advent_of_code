//
// Created by Conor Pappas on 1/14/24.
//

#pragma once

#include <grid.hpp>

namespace crucible::data_types {
    using coordinate = size_t;
    using Point = support::Point<coordinate, 2>;
    using heat = size_t;
    enum class Direction : char {
        Horizontal,
        Vertical
    };
    using Grid = support::Grid<heat>;
    struct Vertex {
        Point position {};
        heat heat_loss {};
        Direction arrived_from {};
    };
    struct Edge {
        heat heat_loss {};
    };

    inline Direction opposite(const Direction direction) {
        return (direction == Direction::Horizontal) ? Direction::Vertical : Direction::Horizontal;
    }
}
