//
// Created by Conor Pappas on 1/15/24.
//

#include "instruction.hpp"

#include <regex>

namespace lagoon::parse {

    const std::regex INSTRUCTION_REGEX(R"(([R,D,L,U])\s*(\d+)\s*\(\#([a-fA-F\d]{6})\))");

    data_types::Instruction parse_instruction(const std::string& str) {
        std::smatch match;
        if(!std::regex_match(str, match, INSTRUCTION_REGEX)) {
            throw std::runtime_error("Invalid step: " + str);
        }
        data_types::Instruction instruction;
        instruction.direction = static_cast<data_types::Direction>(match[1].str()[0]);
        instruction.distance = std::stoi(match[2]);
        instruction.color = std::stoul(match[3], nullptr, 16);
        return instruction;
    }

    std::vector<data_types::Instruction> parse_instructions(const std::vector<std::string>& strs) {
        std::vector<data_types::Instruction> instructions;
        instructions.reserve(strs.size());
        for (const auto& str : strs) {
            instructions.push_back(parse_instruction(str));
        }
        return instructions;
    }
}
