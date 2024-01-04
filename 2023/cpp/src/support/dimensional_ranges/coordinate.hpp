//
// Created by Conor Pappas on 12/24/23.
//

#pragma once

#include "concepts.hpp"
#include "point.hpp"

namespace support::dimensional_ranges {
    template <constexpr dim_size_t dim>
    class Coordinate: public Point<constexpr dim_size_t, dim> {
        using base = Point<constexpr dim_size_t, dim>;
    public:
        static constexpr dim_size_t dimension = dim;

        std::tuple<Coordinate<dim-1>, dim_size_t> destructure_right() const;
        std::tuple<dim_size_t, Coordinate<dim-1>> destructure_left() const;
        template <constexpr dim_size_t D>
        std::tuple<Coordinate<dim-D>, Coordinate<D>> destructure_right() const;
        template <constexpr dim_size_t D>
        std::tuple<Coordinate<D>, Coordinate<dim-D>> destructure_left() const;

        template<typename Range>
        using reference = typename dimensional_range_traits<Range, dim>::full_dereferenced_t;
        template<typename Range>
        using pointer =  typename dimensional_range_traits<Range, dim>::template dereferenced_t_traits<dim-1>::iterator_t;

        template <typename Range>
            requires dimensional_range<Range, dim> && (dim != 0)
        reference<Range> dereference(const Range&);
        template <typename Range>
            requires dimensional_range<Range, dim> && (dim == 0)
        reference<Range> dereference(const Range&);
        template <typename Range>
            requires dimensional_range<Range, dim> && (dim != 0)
        pointer<Range> arrow_dereference(const Range&);

    };

    template<constexpr dim_size_t dim>
    std::tuple<Coordinate<dim - 1>, dim_size_t> Coordinate<dim>::destructure_right() const {
        std::array<dim_size_t, dim - 1> new_coordinates;
        std::copy(base::m_coordinates.begin(), base::m_coordinates.end() - 1, new_coordinates.begin());
        return { {new_coordinates}, base::m_coordinates.back() };
    }

    template<constexpr dim_size_t dim>
    std::tuple<dim_size_t, Coordinate<dim - 1>> Coordinate<dim>::destructure_left() const {
        std::array<dim_size_t, dim - 1> new_coordinates;
        std::copy(base::m_coordinates.begin() + 1, base::m_coordinates.end(), new_coordinates.begin());
        return { base::m_coordinates.front(), {new_coordinates} };
    }

    template<constexpr dim_size_t dim>
    template<constexpr dim_size_t D>
    std::tuple<Coordinate<dim - D>, Coordinate<D>> Coordinate<dim>::destructure_right() const {
        std::array<dim_size_t, dim - D> new_coordinates_left;
        std::copy(base::m_coordinates.begin(), base::m_coordinates.end() - D, new_coordinates_left.begin());
        std::array<dim_size_t, D> new_coordinates_right;
        std::copy(base::m_coordinates.end() - D, base::m_coordinates.end(), new_coordinates_right.begin());
        return { {new_coordinates_left}, {new_coordinates_right} };
    }

    template<constexpr dim_size_t dim>
    template<constexpr dim_size_t D>
    std::tuple<Coordinate<D>, Coordinate<dim - D>> Coordinate<dim>::destructure_left() const {
        return destructure_left<dim-D>(*this);
    }

    template<constexpr dim_size_t dim>
    template<typename Range> requires dimensional_range<Range, dim> && (dim != 0)
    typename Coordinate<dim>::template reference<Range> Coordinate<dim>::dereference(const Range& range) {
        return *arrow_dereference<dim>(range);
    }

    template<constexpr dim_size_t dim>
    template<typename Range> requires dimensional_range<Range, dim> && (dim == 0)
    typename Coordinate<dim>::template reference<Range> Coordinate<dim>::dereference(const Range& range) {
        return range;
    }

    template<constexpr dim_size_t dim>
    template<typename Range> requires dimensional_range<Range, dim> && (dim !=0)
    typename Coordinate<dim>::template pointer<Range> Coordinate<dim>::arrow_dereference(const Range& range) {
        const auto [curried_point, index] = destructure_right();
        const auto curried_range = curried_point.dereference(range);
        auto itr = std::ranges::begin(curried_range);
        std::advance(itr, index);
        return itr;
    }
}
