//
// Created by Conor Pappas on 12/24/23.
//

#pragma once

#include "range_concepts.hpp"

// TODO create namespace?
namespace support {
    using dim_size_t = size_t;

    template<std::ranges::range Range, dim_size_t depth>
    struct nested_range_value_t {
    private:
        using range_traits = range_traits<Range>;
        using iterator_t = typename range_traits::iterator_t;
        using next_dimension_type = typename iterator_traits<iterator_t>::value_type;
    public:
        using type = typename nested_range_value_t<next_dimension_type, depth - 1>::type;
    };

    template<std::ranges::range Range>
    struct nested_range_value_t<Range, 0> {
        using type = Range;
    };

    template<typename Range, dim_size_t dim>
    concept dimensional_range =
        (dim > 0) && std::ranges::range<typename nested_range_value_t<Range, dim - 1>::type>;

    // TODO: can dimension = 0 ?
    // We could add an "empty" type that all 0-dimensional ranges have value_type of
    // Would need to have values for all of the range_traits types
    template<typename Range, dim_size_t dim>
        requires dimensional_range<Range, dim>
    struct dimensional_range_traits: range_traits<Range> {
        static constexpr dim_size_t dimension = dim;

        using range_t = Range;
        template<dim_size_t depth>
            requires (depth < dim)
        using inner_range_t = typename nested_range_value_t<Range, depth>::type;

        template<dim_size_t depth>
            requires (depth < dim)
        using inner_range_traits = dimensional_range_traits<inner_range_t<depth>, dim-depth>;

        // TODO: add trait for reducing dim, staying at the same top level;
        // TODO: add trait for going in a level
        // TODO: or maybe it's another struct? - Let's see how this plays out making a 2D iterator
    };
}
