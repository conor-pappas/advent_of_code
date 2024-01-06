//
// Created by Conor Pappas on 1/5/24.
//

#pragma once

#include <point.hpp>

namespace dish::data_types {

    using point_size_t = size_t;
    using Point = support::Point<point_size_t, 2>;

    enum Direction { NORTH, SOUTH, EAST, WEST };

    constexpr Point to_point(const Direction dir) {
        switch(dir) {
            case NORTH: return -Point::unit(1);
            case SOUTH: return Point::unit(1);
            case EAST: return Point::unit(0);
            case WEST: return -Point::unit(0);
            default: throw std::runtime_error("Invalid direction");
        }
    }
}
