//
// Created by Conor Pappas on 1/15/24.
//

#pragma once

#include "attribute.hpp"

namespace avalanche::data_types {
    using Part = AttributeBundle<rating>;

    struct RatingRange {
        rating min {};
        rating max {};
    };

    using PartRange = AttributeBundle<RatingRange>;

    inline rating size(const RatingRange& rating_range) {
        if (rating_range.min > rating_range.max) return 0;
        return rating_range.max - rating_range.min + 1;
    }
}
