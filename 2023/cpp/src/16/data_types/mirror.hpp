//
// Created by Conor Pappas on 1/6/24.
//

#pragma once

#include<vector>
#include <point.hpp>

namespace lazer::data_types {
    enum class Mirror : char {
        EMPTY = '.',
        FORWARD_MIRROR = '/',
        BACKWARD_MIRROR = '\\',
        HORIZONTAL_SPLITTER = '-',
        VERTICAL_SPLITTER = '|'
    };

    using Direction = support::Point<size_t, 2>;
    const Direction NORTH = -Direction::unit(1);
    const Direction EAST = Direction::unit(0);
    const Direction SOUTH = Direction::unit(1);
    const Direction WEST = -Direction::unit(0);

    std::vector<Direction> propogate_lazer(Mirror entity, const Direction& direction);
}
