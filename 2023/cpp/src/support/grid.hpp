//
// Created by Conor Pappas on 1/4/24.
//

#pragma once

#include "grid_fwd.hpp"

#include <vector>
#include <functional>
#include <iostream>

namespace support {
    // TODO: actually support dims higher than 2 for "rows" / "columns"
    template <typename T, size_t dimension = 2>
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

        [[nodiscard]] iterator begin() noexcept;
        [[nodiscard]] iterator end() noexcept;
        [[nodiscard]] const_iterator begin() const noexcept;
        [[nodiscard]] const_iterator end() const noexcept;

        [[nodiscard]] constexpr bool empty() const noexcept;
        [[nodiscard]] size_type size() const noexcept;

        constexpr void push_back(const_reference);

        Grid<T, dimension> transpose() const requires (dimension == 2);

        friend constexpr bool operator==<T,dimension>(const Grid& lhs, const Grid& rhs);

    private:
        std::vector<value_type> m_sub_grids {};
    };

    template<typename T, size_t dimension>
        requires (dimension > 0)
    Grid<T, dimension>::Grid() = default;

    template<typename T, size_t dimension>
        requires (dimension > 0)
    Grid<T, dimension>::reference Grid<T, dimension>::at(Grid<T, dimension>::size_type index) {
        return m_sub_grids.at(index);
    }

    template<typename T, size_t dimension>
        requires (dimension > 0)
    Grid<T, dimension>::const_reference Grid<T, dimension>::at(Grid<T, dimension>::size_type index) const {
        return m_sub_grids.at(index);
    }

    template<typename T, size_t dimension>
        requires (dimension > 0)
    Grid<T, dimension>::reference Grid<T, dimension>::operator[](Grid<T, dimension>::size_type index) {
        return m_sub_grids[index];
    }

    template<typename T, size_t dimension>
        requires (dimension > 0)
    Grid<T, dimension>::const_reference Grid<T, dimension>::operator[](Grid<T, dimension>::size_type index) const {
        return m_sub_grids[index];
    }

    template<typename T, size_t dimension>
        requires (dimension > 0)
    Grid<T, dimension>::iterator Grid<T, dimension>::begin() noexcept{
        return m_sub_grids.begin();
    }

    template<typename T, size_t dimension>
        requires (dimension > 0)
    Grid<T, dimension>::iterator Grid<T, dimension>::end() noexcept{
        return m_sub_grids.end();
    }

    template<typename T, size_t dimension>
        requires (dimension > 0)
    Grid<T, dimension>::const_iterator Grid<T, dimension>::begin() const noexcept{
        return m_sub_grids.begin();
    }

    template<typename T, size_t dimension>
        requires (dimension > 0)
    Grid<T, dimension>::const_iterator Grid<T, dimension>::end() const noexcept{
        return m_sub_grids.end();
    }

    template<typename T, size_t dimension>
        requires (dimension > 0)
    constexpr bool Grid<T, dimension>::empty() const noexcept {
        return m_sub_grids.empty();
    }

    template<typename T, size_t dimension>
        requires (dimension > 0)
    typename Grid<T, dimension>::size_type Grid<T, dimension>::size() const noexcept {
        return m_sub_grids.size();
    }

    template<typename T, size_t dimension>
        requires (dimension > 0)
    constexpr void Grid<T, dimension>::push_back(Grid<T,dimension>::const_reference element) {
        m_sub_grids.push_back(element);
    }

    // TODO: This should all get replaced with dimensional_ranges
    // TODO: UB if uneven rows
    template <typename T, size_t dimension>
        requires (dimension > 0)
    Grid<T, dimension> Grid<T, dimension>::transpose() const requires (dimension == 2) {
        Grid<T, dimension> result;
        for (size_t j = 0; j < m_sub_grids[0].size(); ++j) {
            value_type row;
            for (size_t i = 0; i < size(); ++i) {
                row.push_back(m_sub_grids[i][j]);
            }
            result.push_back(row);
        }
        return result;
    }

    template<typename T, size_t dimension>
        requires (dimension > 0)
    constexpr bool operator==(const Grid<T,dimension>& lhs, const Grid<T,dimension>& rhs) {
        return lhs.m_sub_grids == rhs.m_sub_grids;
    }
}
