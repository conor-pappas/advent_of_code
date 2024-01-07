//
// Created by Conor Pappas on 1/6/24.
//

#pragma once

#include <vector>

#include "data_types/step.hpp"

namespace library::parse {
    data_types::Step parse_step(const std::string& str);
    std::vector<data_types::Step> parse_steps(const std::string& str);
}
