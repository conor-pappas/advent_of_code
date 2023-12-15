//
// Created by Conor Pappas on 12/15/23.
//

#include "instruction_set.hpp"

#include <utility>

wasteland::InstructionSet::InstructionSet(container_type  instructions):
    instructions(std::move(instructions)) {}

wasteland::InstructionSet::iterator wasteland::InstructionSet::begin() {
    return { instructions.begin(), instructions.end() };
}

std::ostream& wasteland::operator<<(std::ostream& os, const InstructionSet& instruction_set) {
    for(const auto& instruction: instruction_set.instructions) {
        os << static_cast<char>(instruction);
    }
    return os;
}
