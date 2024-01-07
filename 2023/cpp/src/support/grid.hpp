//
// Created by Conor Pappas on 1/4/24.
//

#pragma once

#include "grid_fwd.hpp"

#include <vector>
#include <functional>

#include "point.hpp"

// TODO: This has gotten sloppy. It really should be always maintaining that the dimensions are square by resizing
// consistently across all subgrids. This does require that T is default constructible, but that's not a big deal.
// TODO: Add Allocator template param
namespace support {
    template <typename T, dimension_t dimension = 2>
        requires (dimension > 0)
    class Grid {
    public:
        using size_type = size_t;
        using value_type = grid_value_type_t<T, dimension>;
        using reference = value_type&;
        using const_reference = const value_type&;
        using iterator = typename std::vector<value_type>::iterator;
        using const_iterator = typename std::vector<value_type>::const_iterator;

        Grid();

        reference at(size_type index);
        const_reference at(size_type index) const;
        reference operator[](size_type index);
        const_reference operator[](size_type index) const;

        T& get(const Point<size_type, dimension>&) requires (dimension > 1);
        const T& get(const Point<size_type, dimension>&) const requires (dimension > 1);
        T& get(const Point<size_type, dimension>&) requires (dimension == 1);
        const T& get(const Point<size_type, dimension>&) const requires (dimension == 1);

        bool contains(const Point<size_type, dimension>&) const requires (dimension > 1);
        bool contains(const Point<size_type, dimension>&) const requires (dimension == 1);

        [[nodiscard]] iterator begin() noexcept;
        [[nodiscard]] iterator end() noexcept;
        [[nodiscard]] const_iterator begin() const noexcept;
        [[nodiscard]] const_iterator end() const noexcept;

        [[nodiscard]] constexpr bool empty() const noexcept;
        [[nodiscard]] size_type size() const noexcept;
        // TODO: Add size() for inner dimensions

        [[nodiscard]] size_type width() const noexcept requires (dimension == 2);
        [[nodiscard]] size_type height() const noexcept requires (dimension == 2);

        constexpr void push_back(const_reference);
        // TODO: Figure out how to have signatures with exactly dimension args.
        constexpr void resize(size_type) requires(dimension == 1);
        constexpr void resize(size_type, size_type) requires(dimension == 2);

        // TODO: actually support dims higher than 2 using dimensional_ranges
        Grid transpose() const requires (dimension == 2);

        friend constexpr bool operator==<T,dimension>(const Grid& lhs, const Grid& rhs);

    private:
        std::vector<value_type> m_sub_grids {};
    };

    template<typename T, dimension_t dimension>
        requires (dimension > 0)
    Grid<T, dimension>::Grid() = default;

    template<typename T, dimension_t dimension>
        requires (dimension > 0)
    typename Grid<T, dimension>::reference Grid<T, dimension>::at(Grid<T, dimension>::size_type index) {
        return m_sub_grids.at(index);
    }

    template<typename T, dimension_t dimension>
        requires (dimension > 0)
    typename Grid<T, dimension>::const_reference Grid<T, dimension>::at(Grid<T, dimension>::size_type index) const {
        return m_sub_grids.at(index);
    }

    template<typename T, dimension_t dimension>
        requires (dimension > 0)
    typename Grid<T, dimension>::reference Grid<T, dimension>::operator[](Grid<T, dimension>::size_type index) {
        return m_sub_grids[index];
    }

    template<typename T, dimension_t dimension>
        requires (dimension > 0)
    typename Grid<T, dimension>::const_reference Grid<T, dimension>::operator[](Grid<T, dimension>::size_type index) const {
        return m_sub_grids[index];
    }

    template<typename T, dimension_t dimension> requires (dimension > 0)
    T& Grid<T, dimension>::get(const Point<size_type, dimension>& point)
    requires (dimension > 1) {
        const auto [rest, index] = point.destructure_right();
        return m_sub_grids[index].get(rest);
    }

    template<typename T, dimension_t dimension> requires (dimension > 0)
    const T& Grid<T, dimension>::get(const Point<size_type, dimension>& point) const
    requires (dimension > 1) {
        const auto [rest, index] = point.destructure_right();
        return m_sub_grids[index].get(rest);
    }

    template<typename T, dimension_t dimension> requires (dimension > 0)
    T& Grid<T, dimension>::get(const Point<size_type, dimension>& point)
    requires (dimension == 1) {
        return m_sub_grids[point[0]];
    }

    template<typename T, dimension_t dimension> requires (dimension > 0)
    const T& Grid<T, dimension>::get(const Point<size_type, dimension>& point) const
    requires (dimension == 1) {
        return m_sub_grids[point[0]];
    }

    template<typename T, dimension_t dimension> requires (dimension > 0)
    bool Grid<T, dimension>::contains(const Point<size_type, dimension>& point) const requires (dimension > 1) {
        const auto [rest, index] = point.destructure_right();
        return index >= 0 && index < m_sub_grids.size() && m_sub_grids[index].contains(rest);
    }

    template<typename T, dimension_t dimension> requires (dimension > 0)
    bool Grid<T, dimension>::contains(const Point<size_type, dimension>& point) const requires (dimension == 1) {
        return point[0] >= 0 && point[0] < m_sub_grids.size();
    }

    template<typename T, dimension_t dimension>
        requires (dimension > 0)
    typename Grid<T, dimension>::iterator Grid<T, dimension>::begin() noexcept{
        return m_sub_grids.begin();
    }

    template<typename T, dimension_t dimension>
        requires (dimension > 0)
    typename Grid<T, dimension>::iterator Grid<T, dimension>::end() noexcept{
        return m_sub_grids.end();
    }

    template<typename T, dimension_t dimension>
        requires (dimension > 0)
    typename Grid<T, dimension>::const_iterator Grid<T, dimension>::begin() const noexcept{
        return m_sub_grids.begin();
    }

    template<typename T, dimension_t dimension>
        requires (dimension > 0)
    typename Grid<T, dimension>::const_iterator Grid<T, dimension>::end() const noexcept{
        return m_sub_grids.end();
    }

    template<typename T, dimension_t dimension>
        requires (dimension > 0)
    constexpr bool Grid<T, dimension>::empty() const noexcept {
        return m_sub_grids.empty();
    }

    template<typename T, dimension_t dimension>
        requires (dimension > 0)
    typename Grid<T, dimension>::size_type Grid<T, dimension>::size() const noexcept {
        return m_sub_grids.size();
    }

    template<typename T, dimension_t dimension> requires (dimension > 0)
    typename Grid<T, dimension>::size_type Grid<T, dimension>::width() const noexcept requires (dimension == 2) {
        return m_sub_grids[0].size();
    }

    template<typename T, dimension_t dimension> requires (dimension > 0)
    typename Grid<T, dimension>::size_type Grid<T, dimension>::height() const noexcept requires (dimension == 2) {
        return m_sub_grids.size();
    }

    template<typename T, dimension_t dimension>
        requires (dimension > 0)
    constexpr void Grid<T, dimension>::push_back(const_reference element) {
        m_sub_grids.push_back(element);
    }

    template<typename T, dimension_t dimension> requires (dimension > 0)
    constexpr void Grid<T, dimension>::resize(size_type size) requires (dimension == 1) {
        m_sub_grids.resize(size);
    }

    template<typename T, dimension_t dimension> requires (dimension > 0)
    constexpr void Grid<T, dimension>::resize(size_type width, size_type height) requires (dimension == 2) {
        m_sub_grids.resize(height);
        m_sub_grids[0].resize(width);
        for(auto& sub_grid : m_sub_grids) {
            sub_grid.resize(width);
        }
    }

    // TODO: This should all get replaced with dimensional_ranges
    // TODO: UB if uneven rows
    template <typename T, dimension_t dimension>
        requires (dimension > 0)
    Grid<T, dimension> Grid<T, dimension>::transpose() const requires (dimension == 2) {
        Grid result;
        for (size_t j = 0; j < m_sub_grids[0].size(); ++j) {
            value_type row;
            for (size_t i = 0; i < size(); ++i) {
                row.push_back(m_sub_grids[i][j]);
            }
            result.push_back(row);
        }
        return result;
    }

    template<typename T, dimension_t dimension>
        requires (dimension > 0)
    constexpr bool operator==(const Grid<T,dimension>& lhs, const Grid<T,dimension>& rhs) {
        return lhs.m_sub_grids == rhs.m_sub_grids;
    }
}
