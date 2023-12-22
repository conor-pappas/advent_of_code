//
// Created by Conor Pappas on 12/21/23.
//

#include "grid.hpp"

#include <utility>

namespace maze {
    Grid::Grid() = default;

    void Grid::add_pipe(const Point& point, const Pipe& pipe) {
        m_pipes[point] = pipe;
    }

    std::optional<const Pipe> Grid::get_pipe(const Point& point) const {
        const auto pipe = m_pipes.find(point);
        if (pipe == m_pipes.end()) return {};
        return std::make_optional(pipe->second);
    }

    void Grid::set_start(const Point& point) { m_start = point; }
    const Grid::Point& Grid::get_start() const { return m_start; }
    void Grid::set_width(const coordinate_type width) { m_width = width; }
    Grid::coordinate_type Grid::get_width() const { return m_width; }
    void Grid::set_height(const coordinate_type height) { m_height = height; }
    Grid::coordinate_type Grid::get_height() const { return m_height; }

    Grid::Iterator Grid::begin(Pipe::Direction direction) const {
        return { m_start, direction, *this };
    }

    // TODO: Come back. Maybe we need a sentinel.
    Grid::Iterator Grid::end() const {
        return {};
    }

    Grid::Iterator::Iterator() = default;

    Grid::Iterator::Iterator(const Point& point, const Pipe::Direction direction, Grid grid):
        m_point(point),
        m_direction(direction),
        m_grid(std::move(grid)) {}

    Grid::Iterator::reference Grid::Iterator::operator*() {
        return m_point;
    }

    Grid::Iterator::pointer Grid::Iterator::operator->() {
        return &m_point;
    }

    Grid::Iterator& Grid::Iterator::operator++() {
        m_point += Pipe::to_vector(m_direction);
        const auto flipped_direction = Pipe::negate(m_direction);
        if (const auto pipe = m_grid.get_pipe(m_point)) {
            if (const auto new_direction = pipe.value().follow(flipped_direction)) {
                m_direction = new_direction.value();
            } else {
                *this = m_grid.end();
            }
        } else {
            *this = m_grid.end();
        }
        return *this;
    }

    Grid::Iterator Grid::Iterator::operator++(int) & {
        auto result = *this;
        ++*this;
        return result;
    }

    bool operator==(const Grid& a, const Grid& b) {
        return a.m_pipes == b.m_pipes
            && a.m_start == b.m_start;
    }

    bool operator==(const Grid::Iterator& a, const Grid::Iterator& b) {
        return a.m_direction == b.m_direction
            && a.m_point == b.m_point
            && a.m_grid == b.m_grid;
    }
} // maze
