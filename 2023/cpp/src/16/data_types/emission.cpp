//
// Created by Conor Pappas on 1/7/24.
//

#include "emission.hpp"

namespace lazer::data_types {
    bool Emission::emitting() const {
        return north || east || south || west;
    }

    bool& Emission::operator[](const Direction& direction) {
        return const_cast<bool&>(static_cast<const Emission&>(*this)[direction]);
    }

    const bool& Emission::operator[](const Direction& direction) const {
        if(direction == NORTH) return north;
        if(direction == EAST) return east;
        if(direction == SOUTH) return south;
        if(direction == WEST) return west;
        throw std::runtime_error("Invalid direction");
    }
}
