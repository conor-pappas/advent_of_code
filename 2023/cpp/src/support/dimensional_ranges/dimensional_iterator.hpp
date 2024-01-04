//
// Created by Conor Pappas on 12/26/23.
//

#pragma once

#include <cassert>

#include "concepts.hpp"
#include "coordinate.hpp"
#include "dimension_list.hpp"

namespace support::dimensional_ranges {
    template <std::ranges::forward_range Range, dim_size_t dimension>
        requires dimensional_range<Range, dimension> && (dimension != 0)
    class DimensionalIterator {
        using range_t = typename dimensional_range_traits<Range, dimension>::template dereferenced_t<dimension-1>;
        using iterator_t = typename range_traits<range_t>::iterator_t;

    public:
        // TODO: assert can't be contiguous
        // TODO: honestly difference type should be a coordinate
        using iterator_category = typename std::iterator_traits<iterator_t>::iterator_category;
        using difference_type   = typename range_traits<range_t>::range_difference_t;
        using value_type        = typename range_traits<range_t>::range_value_t;
        using reference         = typename range_traits<range_t>::range_reference_t;
        using pointer           = iterator_t;

        dim_size_t& get_position();
        [[nodiscard]] Coordinate<dimension - 1> get_base_point() const;
        [[nodiscard]] Coordinate<dimension> get_coordinate() const;

        reference operator*() const;
        pointer operator->() const;

        DimensionalIterator& operator++();
        DimensionalIterator<Range, dimension> operator++(int);

        DimensionalIterator& operator--() requires std::bidirectional_iterator<iterator_t>;
        DimensionalIterator<Range, dimension> operator--(int) requires std::bidirectional_iterator<iterator_t>;

        DimensionalIterator& operator+=(difference_type) requires std::random_access_iterator<iterator_t>;
        DimensionalIterator& operator-=(difference_type) requires std::random_access_iterator<iterator_t>;
        // TODO: This is a complicated calculation
        //difference_type operator-=(const DimensionalIterator&) requires std::random_access_iterator<iterator_t>;
        reference operator[](difference_type) const requires std::random_access_iterator<iterator_t>;

        friend bool operator==(const DimensionalIterator&, const DimensionalIterator&);

        friend DimensionalIterator operator+(const DimensionalIterator&, difference_type)
            requires std::random_access_iterator<iterator_t>;
        friend DimensionalIterator operator-(const DimensionalIterator&, difference_type)
            requires std::random_access_iterator<iterator_t>;
        // TODO: This is a complicated calculation
        //friend difference_type operator-(const DimensionalIterator&, const DimensionalIterator&)
        //    requires std::random_access_iterator<iterator_t>;
        friend bool operator<=>(const DimensionalIterator&, const DimensionalIterator&)
            requires std::random_access_iterator<iterator_t>;

    private:
        // TODO: Should we use a coordinate for non random access iterators?
        Coordinate<dimension> m_coordinate;
        std::reference_wrapper<Range> m_base;
        DimensionList<dimension> m_dimensions {};

        void validate_same_base(const DimensionalIterator& other) const;
    };

    template<std::ranges::forward_range Range, dim_size_t dimension>
        requires dimensional_range<Range, dimension> && (dimension != 0)
    dim_size_t& DimensionalIterator<Range, dimension>::get_position()
    {
        return m_coordinate[dimension-1];
    }

    template<std::ranges::forward_range Range, dim_size_t dimension>
        requires dimensional_range<Range, dimension> && (dimension != 0)
    Coordinate<dimension - 1> DimensionalIterator<Range, dimension>::get_base_point() const
    {
        return std::get<0>(m_coordinate.destructure_left());
    }

    template<std::ranges::forward_range Range, dim_size_t dimension>
        requires dimensional_range<Range, dimension> && (dimension != 0)
    Coordinate<dimension> DimensionalIterator<Range, dimension>::get_coordinate() const
    {
        return m_coordinate;
    }

    template<std::ranges::forward_range Range, dim_size_t dimension>
        requires dimensional_range<Range, dimension> && (dimension != 0)
    typename DimensionalIterator<Range, dimension>::reference DimensionalIterator<Range, dimension>::operator*() const
    {
        return m_coordinate.dereference(m_base);
    }

    template<std::ranges::forward_range Range, dim_size_t dimension>
        requires dimensional_range<Range, dimension> && (dimension != 0)
    typename DimensionalIterator<Range, dimension>::pointer DimensionalIterator<Range, dimension>::operator->() const
    {
        return m_coordinate.arrow_dereference(m_base);
    }

    template<std::ranges::forward_range Range, dim_size_t dimension>
        requires dimensional_range<Range, dimension> && (dimension != 0)
    DimensionalIterator<Range, dimension>& DimensionalIterator<Range, dimension>::operator++()
    {
        ++get_position();
        return this;
    }

    template<std::ranges::forward_range Range, dim_size_t dimension>
        requires dimensional_range<Range, dimension> && (dimension != 0)
    DimensionalIterator<Range, dimension> DimensionalIterator<Range, dimension>::operator++(int)
    {
        DimensionalIterator copy = *this;
        ++(*this);
        return copy;
    }

    template<std::ranges::forward_range Range, dim_size_t dimension>
        requires dimensional_range<Range, dimension> && (dimension != 0)
    DimensionalIterator<Range, dimension>& DimensionalIterator<Range, dimension>::operator--()
        requires std::bidirectional_iterator<iterator_t>
    {
        --get_position();
        return this;
    }

    template<std::ranges::forward_range Range, dim_size_t dimension>
        requires dimensional_range<Range, dimension> && (dimension != 0)
    DimensionalIterator<Range, dimension> DimensionalIterator<Range, dimension>::operator--(int)
        requires std::bidirectional_iterator<iterator_t>
    {
        DimensionalIterator copy = *this;
        --(*this);
        return copy;
    }

    template<std::ranges::forward_range Range, dim_size_t dimension>
        requires dimensional_range<Range, dimension> && (dimension != 0)
    DimensionalIterator<Range, dimension>& DimensionalIterator<Range, dimension>::operator+=(difference_type rhs)
        requires std::random_access_iterator<iterator_t>
    {
        get_position() += rhs;
        return this;
    }

    template<std::ranges::forward_range Range, dim_size_t dimension>
        requires dimensional_range<Range, dimension> && (dimension != 0)
    DimensionalIterator<Range, dimension>& DimensionalIterator<Range, dimension>::operator-=(difference_type rhs)
        requires std::random_access_iterator<iterator_t>
    {
        get_position() -= rhs;
        return this;
    }

    template<std::ranges::forward_range Range, dim_size_t dimension>
        requires dimensional_range<Range, dimension> && (dimension != 0)
    typename DimensionalIterator<Range, dimension>::reference DimensionalIterator<Range, dimension>::operator[](difference_type pos) const
        requires std::random_access_iterator<iterator_t>
    {
        return *(*this + pos);
    }

    template<std::ranges::forward_range Range, dim_size_t dimension>
        requires dimensional_range<Range, dimension> && (dimension != 0)
    void DimensionalIterator<Range, dimension>::validate_same_base(const DimensionalIterator& other) const
    {
        assert(get_base_point() == other.get_base_point());
    }

    template<std::ranges::forward_range Range, dim_size_t dimension>
        requires dimensional_range<Range, dimension> && (dimension != 0)
    bool operator==(const DimensionalIterator<Range, dimension>& lhs, const DimensionalIterator<Range, dimension>& rhs)
    {
        return lhs.m_coordinate == rhs.m_coordinate;
    }

    template<std::ranges::forward_range Range, dim_size_t dimension>
        requires dimensional_range<Range, dimension> && (dimension != 0)
    DimensionalIterator<Range, dimension>
    operator+(const DimensionalIterator<Range, dimension>& lhs, typename DimensionalIterator<Range, dimension>::difference_type rhs)
        requires std::random_access_iterator<typename DimensionalIterator<Range, dimension>::iterator_t>
    {
        auto copy = lhs;
        copy += rhs;
        return copy;
    }

    template<std::ranges::forward_range Range, dim_size_t dimension>
        requires dimensional_range<Range, dimension> && (dimension != 0)
    DimensionalIterator<Range, dimension>
    operator-(const DimensionalIterator<Range, dimension>& lhs, typename DimensionalIterator<Range, dimension>::difference_type rhs)
        requires std::random_access_iterator<typename DimensionalIterator<Range, dimension>::iterator_t>
    {
        auto copy = lhs;
        copy -= rhs;
        return copy;
    }

    template<std::ranges::forward_range Range, dim_size_t dimension>
        requires dimensional_range<Range, dimension> && (dimension != 0)
    typename DimensionalIterator<Range, dimension>::difference_type
    operator+(const DimensionalIterator<Range, dimension>& lhs, const DimensionalIterator<Range, dimension>& rhs)
        requires std::random_access_iterator<typename DimensionalIterator<Range, dimension>::iterator_t>
    {
        lhs.validate_same_base(rhs);
        auto copy = lhs;
        copy -= rhs;
        return copy;
    }

    template<std::ranges::forward_range Range, dim_size_t dimension>
        requires dimensional_range<Range, dimension> && (dimension != 0)
    bool operator<=>(const DimensionalIterator<Range, dimension>& lhs, const DimensionalIterator<Range, dimension>& rhs)
        requires std::random_access_iterator<typename DimensionalIterator<Range, dimension>::iterator_t>
    {
        lhs.validate_same_base(rhs);
        return lhs.get_position() <=> rhs.get_position();
    }
}
