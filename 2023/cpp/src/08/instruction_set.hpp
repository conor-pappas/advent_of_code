//
// Created by Conor Pappas on 12/15/23.
//

#pragma once

#include <vector>
#include <ostream>

#include "instruction.hpp"
#include "cycles/cyclic_iterator.hpp"

namespace wasteland {
    class InstructionSet {
    public:
        using container_type = std::vector<const Instruction>;
        using iterator = support::CyclicIterator<container_type::iterator>;

        explicit InstructionSet(container_type  instructions);

        [[nodiscard]] iterator begin() const;

        friend std::ostream& operator<<(std::ostream& os, const InstructionSet& instruction_set);

    private:
        container_type instructions;
    };
};
