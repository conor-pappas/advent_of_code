//
// Created by Conor Pappas on 12/22/23.
//

#pragma once

#include <string>

#include "space.hpp"

namespace cosmic::parse {
    enum class Symbol: char {
        SPACE = '.',
        GALAXY = '#'
    };

    Space parse_space(const std::vector<std::string>& lines);
};
