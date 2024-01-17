//
// Created by Conor Pappas on 1/17/24.
//

#include "ground.hpp"

namespace disintegrate::data_types {
    height resting_height(const Block& block, const Ground& ground) {
        height max_height = 0;
        // TODO: interval library with iterators
        for(coordinate_t y=block.y.min; y <= block.y.max; ++y) {
            for(coordinate_t x=block.x.min; x <= block.x.max; ++x) {
                Point point {x,y};
                const auto& support_point = ground.get(point);
                if(support_point.height > max_height) {
                    max_height = support_point.height;
                }
            }
        }
        return max_height;
    }

    std::shared_ptr<Block> place_onto(const std::shared_ptr<Block>& block, SupportPoint& support) {
        const auto existing_block = support.top_block;
        support.top_block = block;
        support.height = block->z.max;
        return existing_block;
    }

    std::set<std::shared_ptr<Block>> fall_onto(const std::shared_ptr<Block>& block, Ground& ground) {
        const auto height = resting_height(*block, ground);
        fall_to(*block, height + 1);

        std::set<std::shared_ptr<Block>> result;
        for(size_t y=block->y.min; y <= block->y.max; ++y) {
            for(size_t x=block->x.min; x <= block->x.max; ++x) {
                auto& support = ground.get({x,y});
                if(support.height == height) {
                    result.insert(place_onto(block, support));
                } else {
                    place_onto(block, support);
                }
            }
        }
        return result;
    }
}
