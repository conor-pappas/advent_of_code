//
// Created by Conor Pappas on 12/15/23.
//

#pragma once

#include "cyclic_view.hpp"

namespace support {
    template<typename Iterator>
    struct CycleFinder {
        using diff_t = typename Iterator::difference_type;

        struct Cycle {
            std::ranges::subrange<Iterator> tail {};
            CyclicView<Iterator> loop {};
            diff_t cycle_length { 0 };
            diff_t tail_length { 0 };
        };

        static std::optional<Cycle> find(const Iterator& ittr, const diff_t limit = std::numeric_limits<diff_t>::max()) {
            Iterator slow_ptr = ittr;
            Iterator fast_ptr = ittr;
            diff_t traveled = 0;
            do {
                ++slow_ptr;
                ++fast_ptr;
                ++fast_ptr;
                ++traveled;
            } while (slow_ptr != fast_ptr && traveled != limit);

            if(traveled == limit) return {};

            auto [loop_begin, tail_length] = intersection(ittr, slow_ptr);
            Cycle cycle;
            CyclicView view(loop_begin);
            cycle.tail = std::ranges::subrange(ittr, loop_begin);
            cycle.loop = view;
            cycle.cycle_length = view.size();
            cycle.tail_length = tail_length;

            return std::make_optional(cycle);
        }

        static std::tuple<Iterator, diff_t> intersection(Iterator ittr1, Iterator ittr2) {
            diff_t distance = 0;
            while (ittr1 != ittr2) {
                ++ittr1;
                ++ittr2;
                ++distance;
            }
            return std::tuple<Iterator, diff_t>(ittr1, distance);
        }
    };
};
