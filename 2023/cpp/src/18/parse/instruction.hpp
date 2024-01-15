//
// Created by Conor Pappas on 1/15/24.
//

#pragma once

#include <vector>

#include "data_types/instruction.hpp"

namespace lagoon::parse {
    data_types::RawInstruction parse_instruction(const std::string& str);
    std::vector<data_types::RawInstruction> parse_instructions(const std::vector<std::string>&);
}
