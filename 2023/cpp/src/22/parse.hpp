//
// Created by Conor Pappas on 1/17/24.
//

#pragma once

#include <string>
#include <vector>

#include "data_types/block.hpp"

namespace disintegrate::parse {
    data_types::Block parse_block(const std::string& str);
    std::vector<data_types::Block> parse_block(const std::vector<std::string>&);
}
