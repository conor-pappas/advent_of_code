//
// Created by Conor Pappas on 1/15/24.
//

#pragma once

#include <optional>

#include "part.hpp"
#include "label.hpp"

namespace avalanche::data_types {
    using attribute_type = rating Part::*;

    enum class Compare : char {
        Greater = '>',
        Less = '<'
    };

    struct Rule {
        attribute_type attribute {};
        Compare compare {};
        rating value {};
        label label {};
    };

    bool matches(const Rule&, const Part&);
    std::optional<label> follow(const Rule&, const Part&);


}
