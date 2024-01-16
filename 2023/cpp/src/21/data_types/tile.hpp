//
// Created by Conor Pappas on 1/16/24.
//

#pragma once

#include <grid.hpp>

namespace step::data_types {
    enum class Tile: char {
        Plot = '.',
        Rock = '#',
        Start = 'S'
    };
}
