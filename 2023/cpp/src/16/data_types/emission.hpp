//
// Created by Conor Pappas on 1/7/24.
//

#pragma once

#include "mirror.hpp"

namespace lazer::data_types {
    struct Emission {
        bool north;
        bool east;
        bool south;
        bool west;

        [[nodiscard]] bool emitting() const;

        bool& operator[](const Direction& direction);
        const bool& operator[](const Direction& direction) const;
    };
}
