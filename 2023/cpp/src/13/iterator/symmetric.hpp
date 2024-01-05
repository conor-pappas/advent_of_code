//
// Created by Conor Pappas on 12/23/23.
//

#pragma once

#include <functional>

#include "range_concepts.hpp"

namespace incidence::iterator {

    template<typename T>
    concept range_type = std::ranges::bidirectional_range<T>;

    template<range_type Range>
    using iterator_t = typename support::range_traits<Range>::iterator;

    template<range_type Range>
    using reference_t = const typename support::range_traits<Range>::range_reference;

    template<range_type Range, std::integral result>
    using value_compare = std::function<result(reference_t<Range>, reference_t<Range>)>;

    template<range_type Range>
    bool is_odd_symmetric(iterator_t<Range>, const Range&);

    template<range_type Range>
    bool is_even_symmetric(iterator_t<Range> left, iterator_t<Range> right, const Range&);

    template<range_type Range, std::integral result = size_t>
    result odd_symmetric_difference(iterator_t<Range>, const Range&, value_compare<Range, result>);

    template<range_type Range, std::integral result = size_t>
    result even_symmetric_difference(iterator_t<Range> left, iterator_t<Range> right, const Range&, value_compare<Range, result>);

    template<range_type Range>
    bool is_odd_symmetric(iterator_t<Range> itr, const Range& range) {
        return is_even_symmetric(itr, itr, range);
    }

    template<range_type Range>
    bool is_even_symmetric(iterator_t<Range> left, iterator_t<Range> right, const Range& range) {
        while(left >= std::ranges::begin(range) && right < std::ranges::end(range)) {
            if(*left != *right) return false;
            --left;
            ++right;
        }
        return true;
    }

    template<range_type Range, std::integral result>
    result odd_symmetric_difference(iterator_t<Range> itr, const Range& range, value_compare<Range, result> compare) {
        return even_symmetric_difference(itr, itr, range, compare);
    }

    template<range_type Range, std::integral result>
    result even_symmetric_difference(iterator_t<Range> left, iterator_t<Range> right, const Range& range, value_compare<Range, result> compare) {
        result mistakes = 0;
        while(left >= std::ranges::begin(range) && right < std::ranges::end(range)) {
            mistakes += compare(*left, *right);
            --left;
            ++right;
        }
        return mistakes;
    }
}
