//
// Created by Conor Pappas on 1/4/24.
//

#pragma once

#include <vector>
#include <string>

#include "data_types/platform.hpp"

namespace dish::parse {
    data_types::Platform parse_platform(const std::vector<std::string>&);
}
