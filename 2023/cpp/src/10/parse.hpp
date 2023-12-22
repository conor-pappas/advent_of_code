//
// Created by Conor Pappas on 12/21/23.
//

#pragma once

#include "grid.hpp"

namespace maze::parse {
    enum class Symbol: char {
        VERTICAL = '|',
        HORIZONTAL = '-',
        NE_CORNER = 'L',
        NW_CORNER = 'J',
        SE_CORNER = 'F',
        SW_CORNER = '7',
        GROUND = '.',
        START = 'S'
    };
    Grid parse_grid(const std::vector<std::string>& lines);

    Pipe parse_pipe(const char&);

};
