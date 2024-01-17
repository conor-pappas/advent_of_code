//
// Created by Conor Pappas on 1/16/24.
//

#pragma once

#include <set>

#include <point.hpp>
#include <grid.hpp>

#include "block.hpp"

namespace disintegrate::data_types {

    using Point = support::Point<coordinate_t,2>;
    struct SupportPoint {
        height height {};
        std::shared_ptr<Block> top_block {};
    };

    using Ground = support::Grid<SupportPoint>;

    height resting_height(const Block&, const Ground&);
    std::shared_ptr<Block> place_onto(const std::shared_ptr<Block>&, SupportPoint&);
    std::set<std::shared_ptr<Block>> fall_onto(const std::shared_ptr<Block>&, Ground&);
}
