//
// Created by Conor Pappas on 12/23/23.
//

#pragma once

#include "range_concepts.hpp"

namespace incidence::iterator {

    template<std::ranges::bidirectional_range Range>
    using Itr = support::range_traits<Range>::iterator_t;

    template<std::ranges::bidirectional_range Range>
    bool is_odd_symmetric(Itr<Range>, const Range&);

    template<std::ranges::bidirectional_range Range>
    bool is_even_symmetric(Itr<Range> left, Itr<Range> right, const Range&);

    template<std::ranges::bidirectional_range Range>
    bool is_odd_symmetric(Itr<Range> itr, const Range& range) {
        return is_even_symmetric(itr, itr, range);
    }

    template<std::ranges::bidirectional_range Range>
    bool is_even_symmetric(Itr<Range> left, Itr<Range> right, const Range& range) {
        while(left >= std::ranges::begin(range) && right < std::ranges::end(range)) {
            if(*left != *right) return false;
            --left;
            ++right;
        }
        return true;
    }
}
