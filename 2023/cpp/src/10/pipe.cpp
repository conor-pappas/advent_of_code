//
// Created by Conor Pappas on 12/21/23.
//

#include "pipe.hpp"

namespace maze {
    const std::array<Pipe::Direction, 2>& Pipe::get_directions() const {
        return m_directions;
    }

    std::optional<Pipe::Direction> Pipe::follow(const Direction& input) const {
        if (input == m_directions[0]) return m_directions[1];
        if (input == m_directions[1]) return m_directions[0];
        return {};
    }

    // NOTE: We are using computer graphic coordinates, hence swapped north/south.
    Pipe::Vector Pipe::to_vector(const Direction& direction) {
        switch(direction) {
            case Direction::NORTH: return {0, -1};
            case Direction::EAST: return {1, 0};
            case Direction::SOUTH: return {0, 1};
            case Direction::WEST: return {-1, 0};
            default: throw std::invalid_argument("Invalid direction");
        }
    }

    Pipe::Direction Pipe::to_direction(const Vector& vector) {
        // TODO: Add templated basis vectors to "point.hpp"
        if(vector == Vector(0,1)) return Direction::NORTH;
        if(vector == Vector(1,0)) return Direction::EAST;
        if(vector == Vector(0,-1)) return Direction::SOUTH;
        if(vector == Vector(-1,0)) return Direction::WEST;
        throw std::invalid_argument("Invalid vector");
    }

    Pipe::Direction Pipe::negate(const Direction& direction) {
        switch(direction) {
            case Direction::NORTH: return Direction::SOUTH;
            case Direction::EAST: return Direction::WEST;
            case Direction::SOUTH: return Direction::NORTH;
            case Direction::WEST: return Direction::EAST;
            default: throw std::invalid_argument("Invalid direction");
        }
    }

    Pipe::Direction Pipe::direciton_from_char(const char& c) {
        switch(c) {
            case 'N': return Direction::NORTH;
            case 'E': return Direction::EAST;
            case 'S': return Direction::SOUTH;
            case 'W': return Direction::WEST;
            default: throw std::invalid_argument("Invalid character");
        }
    }

    char Pipe::char_from_direction(const Direction& d) {
        switch(d) {
            case Direction::NORTH: return 'N';
            case Direction::EAST: return 'E';
            case Direction::SOUTH: return 'S';
            case Direction::WEST: return 'W';
            default: throw std::invalid_argument("Invalid direction");
        }
    }

    bool operator==(const Pipe& a, const Pipe& b) {
        return a.m_directions == b.m_directions;
    }
};
