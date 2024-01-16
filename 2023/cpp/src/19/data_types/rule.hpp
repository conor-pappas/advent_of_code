//
// Created by Conor Pappas on 1/15/24.
//

#pragma once

#include <optional>

#include "parts.hpp"
#include "label.hpp"

namespace avalanche::data_types {
    enum class Compare : char {
        Greater = '>',
        Less = '<'
    };

    struct Rule {
        AttributeType attribute {};
        Compare compare {};
        rating value {};
        label label {};
    };

    bool matches(const Rule&, const Part&);
    std::optional<label> follow(const Rule&, const Part&);

    PartRange follows(const Rule&, PartRange);
    PartRange skips(const Rule&, PartRange);


}
