//
// Created by Conor Pappas on 12/31/23.
//

#pragma once

#include <ranges>

#include "dimensional_iterator.hpp"
#include "dimension_list.hpp"

namespace support::dimensional_ranges {

    template <std::ranges::forward_range Range, dim_size_t dimension>
        requires dimensional_range<Range, dimension> && (dimension != 0)
    class DimensionalView : public std::ranges::view_interface<DimensionalView<Range, dimension>> {
    public:
        DimensionalView() requires std::default_initializable<Range>;
        explicit DimensionalView(const Range&);
        DimensionalView(const Range&, const DimensionList<dimension>&);

        DimensionalIterator<DimensionalView<Range, dimension>, dimension> begin() const;
        DimensionalIterator<DimensionalView<Range, dimension>, dimension> end() const;


    private:
        std::reference_wrapper<Range> m_base {};
        DimensionList<dimension> m_dimensions {};
        DimensionalView inner_view() requires (dimension != 1);
    };

    template<std::ranges::forward_range Range, dim_size_t dimension>
    requires dimensional_range<Range, dimension> &&
             (dimension != 0)DimensionalIterator<DimensionalView<Range, dimension>, dimension>
    DimensionalView<Range, dimension>::begin() const {
        DimensionalIterator<DimensionalView<Range, dimension>, dimension> begin
    }

    template<std::ranges::forward_range Range, dim_size_t dimension>
        requires dimensional_range<Range, dimension> && (dimension != 0)
    DimensionalView<Range, dimension>::DimensionalView()
        requires std::default_initializable<Range> = default;

    template<std::ranges::forward_range Range, dim_size_t dimension>
        requires dimensional_range<Range, dimension> && (dimension != 0)
    DimensionalView<Range, dimension>::DimensionalView(const Range& base):
        m_base(base) {}

    template<std::ranges::forward_range Range, dim_size_t dimension>
        requires dimensional_range<Range, dimension> && (dimension != 0)
    DimensionalView<Range, dimension>::DimensionalView(const Range& base, std::array<dim_size_t, dimension> dimensions):
        m_base(base),
        m_dimensions(dimensions) {}

    template<std::ranges::forward_range Range, dim_size_t dimension>
        requires dimensional_range<Range, dimension> && (dimension != 0)
    DimensionalView<Range, dimension> DimensionalView<Range, dimension>::inner_view() requires (dimension != 1) {
        std::array<dim_size_t, dimension-1> new_dimensions;
        std::copy(m_dimensions.begin() + 1, m_dimensions.end(), new_dimensions.begin());
        return DimensionalView<Range, dimension-1>(m_base, m_dimensions);
    }
}
