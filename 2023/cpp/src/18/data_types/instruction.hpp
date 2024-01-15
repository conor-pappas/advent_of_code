//
// Created by Conor Pappas on 1/15/24.
//

#pragma once

namespace lagoon::data_types {
    using distance_t = unsigned int;

    enum class Direction : char {
        Right = 'R',
        Left = 'L',
        Up = 'U',
        Down = 'D'
    };

    struct Instruction {
        Direction direction {};
        distance_t distance {};
    };

    struct RawInstruction {
        Instruction part_1 {};
        Instruction part_2 {};
    };

    inline Direction to_direction(char c) {
        switch(c) {
            case '0': return Direction::Right;
            case '1': return Direction::Down;
            case '2': return Direction::Left;
            case '3': return Direction::Up;
            default: throw std::runtime_error("Invalid direction");
        }
    }
}
