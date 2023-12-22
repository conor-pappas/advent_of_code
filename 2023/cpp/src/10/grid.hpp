//
// Created by Conor Pappas on 12/21/23.
//

#pragma once

#include <unordered_map>
#include "pipe.hpp"

namespace maze {
    class Grid {
    public:
        // TODO: unsigned? (issues with Vector, so keeping it all the same type for now)
        using coordinate_type = int;
        using Point = support::Point<coordinate_type, 2>;
        class Iterator;
        Grid();

        void add_pipe(const Point&, const Pipe&);
        [[nodiscard]] std::optional<const Pipe> get_pipe(const Point&) const;

        void set_start(const Point&);
        [[nodiscard]] const Point& get_start() const;

        void set_width(coordinate_type);
        [[nodiscard]] coordinate_type get_width() const;
        void set_height(coordinate_type);
        [[nodiscard]] coordinate_type get_height() const;

        [[nodiscard]] Iterator begin(Pipe::Direction) const;
        [[nodiscard]] Iterator end() const;

        friend bool operator==(const Grid&, const Grid&);

    private:
        std::unordered_map<Point, Pipe, Point::Hasher> m_pipes;
        Point m_start;
        coordinate_type m_width {};
        coordinate_type m_height {};
    };

    class Grid::Iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = ptrdiff_t;
        using value_type        = Point;
        using pointer           = value_type*;
        using reference         = value_type&;

        Iterator();
        Iterator(const Point&, Pipe::Direction, Grid);

        [[nodiscard]] reference operator*();
        [[nodiscard]] pointer operator->();
        Iterator& operator++();
        Iterator operator++(int) &;

        friend bool operator==(const Iterator& a, const Iterator& b);

    private:
        value_type m_point {};
        Pipe::Direction m_direction {};
        Grid m_grid {};
    };
} // maze
