//
// Created by Conor Pappas on 1/7/24.
//

#include "mirror.hpp"

namespace lazer::data_types {
    std::vector<Direction> propogate_lazer(Mirror entity, const Direction& direction) {
        std::vector<Direction> result;
        switch(entity) {
            case Mirror::EMPTY:
                result.push_back(direction);
                break;
            case Mirror::FORWARD_MIRROR:
                if(direction == NORTH) result.push_back(EAST);
                else if(direction == EAST) result.push_back(NORTH);
                else if(direction == SOUTH) result.push_back(WEST);
                else if(direction == WEST) result.push_back(SOUTH);
                break;
            case Mirror::BACKWARD_MIRROR:
                if(direction == NORTH) result.push_back(WEST);
                else if(direction == EAST) result.push_back(SOUTH);
                else if(direction == SOUTH) result.push_back(EAST);
                else if(direction == WEST) result.push_back(NORTH);
                break;
            case Mirror::HORIZONTAL_SPLITTER:
                if(direction == NORTH || direction == SOUTH) {
                    result.push_back(EAST);
                    result.push_back(WEST);
                } else {
                    result.push_back(direction);
                }
                break;
            case Mirror::VERTICAL_SPLITTER:
                if(direction == EAST || direction == WEST) {
                    result.push_back(NORTH);
                    result.push_back(SOUTH);
                } else {
                    result.push_back(direction);
                }
                break;
            default:
                throw std::runtime_error("Invalid entity");
        }
        return result;
    }
}
