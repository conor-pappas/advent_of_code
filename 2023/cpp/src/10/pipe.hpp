//
// Created by Conor Pappas on 12/21/23.
//

#pragma once

#include <array>
#include <optional>

#include "point.hpp"

namespace maze {
    class Pipe {
    public:
        // TODO: Use char. We are keeping the type the same as Grid::Point for now to avoid casting.
        using Vector = support::Point<int, 2>;
        enum class Direction: unsigned char;

        Pipe();
        explicit Pipe(const std::array<Direction, 2>&);
        Pipe(const Direction&, const Direction&);
        explicit Pipe(const std::array<char, 2>&);
        Pipe(const char&, const char&);

        [[nodiscard]] std::optional<Direction> follow(const Direction&) const;

        static Vector to_vector(const Direction&);
        static Direction to_direction(const Vector&);
        static Direction negate(const Direction&);

        static Direction direciton_from_char(const char&);
        static char char_from_direction(const Direction&);

        friend bool operator==(const Pipe&, const Pipe&);

    private:
        std::array<Direction, 2> m_directions {};
    };

    enum class Pipe::Direction: unsigned char{
        NORTH = 0,
        EAST = 1,
        SOUTH = 2,
        WEST = 3
    };

};
