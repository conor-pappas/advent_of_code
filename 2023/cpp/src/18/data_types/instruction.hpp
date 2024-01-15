//
// Created by Conor Pappas on 1/15/24.
//

#pragma once

namespace lagoon::data_types {
    using hex = unsigned int;
    using distance = unsigned int;

    enum class Direction : char {
        Right = 'R',
        Left = 'L',
        Up = 'U',
        Down = 'D'
    };

    struct Instruction {
        Direction direction {};
        distance distance {};
        hex color {};
    };
}
