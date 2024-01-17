//
// Created by Conor Pappas on 1/16/24.
//

#pragma once

#include <string>

#include <iostream>

namespace disintegrate::data_types {
    template<typename T>
    struct Interval {
        T min {};
        T max {};
    };

    // still compensating for not fixing Grid to use signed types
    using coordinate_t = size_t;
    using height = long;

    struct Block {
        Interval<coordinate_t> x {};
        Interval<coordinate_t> y {};
        Interval<height> z {};
        int label {};
    };

    template<typename T>
    void shift(Interval<T>& interval, T distance);

    template<typename T>
    std::ostream& operator<<(std::ostream& os, const Interval<T>& interval);

    void fall(Block& block, height distance);
    void fall_to(Block& block, height final);

    std::ostream& operator<<(std::ostream& os, const Block& block);

    template<typename T>
    void shift(Interval<T>& interval, const T distance) {
        interval.min += distance;
        interval.max += distance;
    }

    template<typename T>
    std::ostream& operator<<(std::ostream& os, const Interval<T>& interval) {
        os << "[" << interval.min << "," << interval.max << "]";
        return os;
    }
}
