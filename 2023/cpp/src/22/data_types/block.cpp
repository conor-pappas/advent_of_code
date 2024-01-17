//
// Created by Conor Pappas on 1/17/24.
//

#include "block.hpp"

namespace disintegrate::data_types {
    void fall(Block& block, const height distance) {
        shift(block.z, distance);
    }

    void fall_to(Block& block, const height final) {
        fall(block, final - block.z.min);
    }

    std::ostream& operator<<(std::ostream& os, const Block& block) {
        return os << "[" << block.label << "]~" << block.x << "," << block.y << "," << block.z;

    };
}
