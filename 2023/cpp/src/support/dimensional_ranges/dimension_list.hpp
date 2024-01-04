//
// Created by Conor Pappas on 12/31/23.
//

#pragma once

#include "concepts.hpp"
#include "range_concepts.hpp"

namespace support::dimensional_ranges {

    // TODO: I intend this to be more constexpr lists. But technically you could pass any range_of<dim_size_t>.
    // This started to lead me down the type-erasure rabbit hole. Let's... not. We are going to just keep
    // a std::array and deal with the bloat from copying it around.
    // This could also change the interface a lot:
    //  - Should dimension param should even be passed?
    //  - Either need type erasure or passing in range types as template params
    // Or we could just create two separate classes. One for storage, and the other for ranges. :smile:
    template <dim_size_t dimension>
    class DimensionList {
    public:
        constexpr DimensionList(std::initializer_list<dim_size_t>);
    private:
        std::array<dim_size_t, dimension> m_dimensions {};
    };

    template<dim_size_t dimension>
    constexpr DimensionList<dimension>::DimensionList(std::initializer_list<dim_size_t> dimensions):
        m_dimensions(dimensions) {}
}
