//
// Created by Conor Pappas on 12/23/23.
//

#pragma once

#include <ranges>
#include <algorithm>

#include "concepts.hpp"
#include "coordinate.hpp"
#include "dimensional_view.hpp"

namespace support::dimensional_ranges {

    template<std::ranges::forward_range Range, dim_size_t a = 0, dim_size_t b = 1>
        requires dimensional_range<Range, std::max(a,b)+1>
    class TransposeView : public std::ranges::view_interface<TransposeView<Range, a, b>>{
        class TransposeIterator;
    public:
        using iterator = TransposeIterator;

        TransposeView() requires std::default_initializable<Range>;

        iterator begin() const;
        iterator end() const;
    };
}
