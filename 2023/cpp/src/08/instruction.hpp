//
// Created by Conor Pappas on 12/15/23.
//

#pragma once

namespace wasteland {
    class Instruction {
    public:
        enum class Direction: char {
            LEFT = 'L',
            RIGHT = 'R'
        };

        explicit Instruction(const char&);
        explicit operator char() const;

        bool operator==(const Instruction&) const;

        [[nodiscard]] Direction get_direction() const;

    private:
        Direction direction = Direction::LEFT;
    };
};
