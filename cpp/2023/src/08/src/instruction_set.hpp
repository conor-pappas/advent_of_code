//
// Created by Conor Pappas on 12/15/23.
//

#pragma once

#include <vector>
#include <ostream>

#include "instruction.hpp"
#include "cyclic_iterator.hpp"

namespace wasteland {
    class InstructionSet {
    public:
        using container_type = std::vector<Instruction>;
        using iterator = CyclicIterator<Instruction, std::vector<Instruction>::iterator>;

        explicit InstructionSet(container_type  instructions);

        iterator begin();

        friend std::ostream& operator<<(std::ostream& os, const InstructionSet& instruction_set);

    private:
        container_type instructions;
    };
};
