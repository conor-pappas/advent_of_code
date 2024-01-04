//
// Created by Conor Pappas on 12/24/23.
//

#pragma once

//#include <algorithm>

#include "range_concepts.hpp"

namespace support::dimensional_ranges {
    using dim_size_t = size_t;

    template<std::ranges::range Range, dim_size_t depth>
    struct dereferenced_t {
    private:
        using range_traits = range_traits<Range>;
    public:
        using type = typename dereferenced_t<typename range_traits::range_value_t, depth - 1>::type;
    };

    template<std::ranges::range Range>
    struct dereferenced_t<Range, 0> {
        using type = Range;
    };

    template<typename Range, dim_size_t dim>
    concept dimensional_range = std::ranges::range<typename dereferenced_t<Range, dim>::type>;

    template<typename Range, dim_size_t dim>
    struct dimensional_range_traits: range_traits<Range> {
        static constexpr dim_size_t dimension = dim;

        template<dim_size_t depth>
            requires (depth <= dim)
        using dereferenced_t = typename dereferenced_t<Range, depth>::type;

        // TODO: test. < should be right as <= would assume the last dereferenced_t is a range;
        template<dim_size_t depth>
            requires (depth < dim)
        using dereferenced_t_traits = dimensional_range_traits<dereferenced_t<dim>, dim-depth>;

        using full_dereferenced_t = typename dereferenced_t<dim>::type;
    };

    // TODO: add trait for reducing dim, staying at the same top level;
    // TODO: add trait for going in a level
    // TODO: or maybe it's another struct? - Let's see how this plays out making a 2D iterator
}
