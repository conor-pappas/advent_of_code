//
// Created by Conor Pappas on 12/18/23.
//

#pragma once

#include <vector>
#include <string>

#include "input.hpp"
#include "history.hpp"

namespace oasis {
    class parse {
    public:
        static std::vector<History> parse_input(const std::vector<std::string>& lines);
        static History parse_history(const std::string& line);
    };
};
