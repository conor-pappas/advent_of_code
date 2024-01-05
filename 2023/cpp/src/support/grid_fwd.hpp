//
// Created by Conor Pappas on 1/4/24.
//

#pragma once

namespace support {
    template<typename T, size_t dimension>
        requires (dimension > 0)
    class Grid;

    template<typename T, size_t dimension>
        requires (dimension > 0)
    constexpr bool operator==(const Grid<T,dimension>& lhs, const Grid<T,dimension>& rhs);

    template<typename T, size_t dimension>
    struct grid_value_type {
        using type = Grid<T, dimension - 1>;
    };

    template<typename T>
    struct grid_value_type<T,1> {
        using type = T;
    };

    template<typename T, size_t dimension>
    using grid_value_type_t = typename grid_value_type<T, dimension>::type;
}
