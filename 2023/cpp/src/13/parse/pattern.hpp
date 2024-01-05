//
// Created by Conor Pappas on 12/23/23.
//

#pragma once

#include <vector>
#include <string>

#include "data_types/pattern.hpp"

namespace incidence::parse {

    std::vector<data_types::Pattern> parse_patterns(const std::vector<std::string>&);
    data_types::Pattern parse_pattern(const std::vector<std::string>&);

}
