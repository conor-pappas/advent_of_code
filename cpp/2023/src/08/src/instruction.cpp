//
// Created by Conor Pappas on 12/15/23.
//

#include "instruction.hpp"

wasteland::Instruction::Instruction(const char& direction):
    direction(static_cast<Direction>(direction)) {}

wasteland::Instruction::operator char() const {
    return static_cast<char>(direction);
}

wasteland::Instruction::Direction wasteland::Instruction::get_direction() const {
    return direction;
};
