//
// Created by Conor Pappas on 1/15/24.
//

#include "instruction.hpp"

#include <regex>

namespace lagoon::parse {

    const std::regex INSTRUCTION_REGEX(R"(([R,D,L,U])\s*(\d+)\s*\(\#([a-fA-F\d]{5})([a-fA-F\d])\))");

    data_types::RawInstruction parse_instruction(const std::string& str) {
        std::smatch match;
        if(!std::regex_match(str, match, INSTRUCTION_REGEX)) {
            throw std::runtime_error("Invalid step: " + str);
        }
        data_types::Instruction part_1_instruction;
        part_1_instruction.direction = static_cast<data_types::Direction>(match[1].str()[0]);
        part_1_instruction.distance = std::stoi(match[2]);
        data_types::Instruction part_2_instruction;
        part_2_instruction.distance = std::stoul(match[3], nullptr, 16);
        part_2_instruction.direction = data_types::to_direction(match[4].str()[0]);

        return {part_1_instruction, part_2_instruction };
    }

    std::vector<data_types::RawInstruction> parse_instructions(const std::vector<std::string>& strs) {
        std::vector<data_types::RawInstruction> instructions;
        instructions.reserve(strs.size());
        for (const auto& str : strs) {
            instructions.push_back(parse_instruction(str));
        }
        return instructions;
    }
}
