//
// Created by Conor Pappas on 12/21/23.
//

#pragma once

#include <array>
#include <optional>
#include <__algorithm/ranges_sort.h>

#include "point.hpp"

namespace maze {
    class Pipe {
    public:
        // TODO: Use char. We are keeping the type the same as Grid::Point for now to avoid casting.
        using Vector = support::Point<int, 2>;
        enum class Direction: unsigned char;

        constexpr Pipe();
        constexpr explicit Pipe(const std::array<Direction, 2>& directions);
        constexpr Pipe(const Direction& d1, const Direction& d2);
        constexpr explicit Pipe(const std::array<char, 2>& chars);
        constexpr Pipe(const char& c1, const char& c2);

        [[nodiscard]] const std::array<Direction, 2>& get_directions() const;

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

    constexpr Pipe::Pipe() = default;

    constexpr Pipe::Pipe(const std::array<Direction, 2>& directions):
        m_directions(directions) {
        std::ranges::sort(m_directions);
    }

    constexpr Pipe::Pipe(const Direction& d1, const Direction& d2):
        Pipe(std::array<Direction,2>({d1, d2})) {}

    constexpr Pipe::Pipe(const std::array<char, 2>& chars):
        Pipe(chars[0], chars[1]) {}

    constexpr Pipe::Pipe(const char& c1, const char& c2):
        Pipe(direciton_from_char(c1), direciton_from_char(c2)) {}

    enum class Pipe::Direction: unsigned char {
        NORTH,
        EAST,
        SOUTH,
        WEST
    };

    struct pipes {
        static constexpr Pipe vertical = { Pipe::Direction::NORTH, Pipe::Direction::SOUTH };
        static constexpr Pipe horizontal = { Pipe::Direction::EAST, Pipe::Direction::WEST };
        static constexpr Pipe north_east = { Pipe::Direction::NORTH, Pipe::Direction::EAST };
        static constexpr Pipe north_west = { Pipe::Direction::NORTH, Pipe::Direction::WEST };
        static constexpr Pipe south_east = { Pipe::Direction::SOUTH, Pipe::Direction::EAST };
        static constexpr Pipe south_west = { Pipe::Direction::SOUTH, Pipe::Direction::WEST };
    };
};
