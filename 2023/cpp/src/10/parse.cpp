//
// Created by Conor Pappas on 12/21/23.
//

#include "parse.hpp"

#include <cassert>
#include <vector>
#include <string>

namespace maze {
    Grid parse::parse_grid(const std::vector<std::string>& lines) {
        assert(!lines.empty());
        Grid grid;
        grid.set_height(static_cast<Grid::coordinate_type>(lines.size()));
        grid.set_width(static_cast<Grid::coordinate_type>(lines[0].size()));
        for(size_t y = 0; y<lines.size(); ++y) {
            for(size_t x = 0; x<lines[y].size(); ++x) {
                char c = lines[y][x];
                Grid::Point point(static_cast<int>(x), static_cast<int>(y));
                switch(static_cast<Symbol>(c)) {
                    case Symbol::VERTICAL:
                    case Symbol::HORIZONTAL:
                    case Symbol::NE_CORNER:
                    case Symbol::NW_CORNER:
                    case Symbol::SE_CORNER:
                    case Symbol::SW_CORNER:
                        grid.add_pipe(point, parse_pipe(c));
                        break;
                    case Symbol::START:
                        grid.set_start(point);
                        break;
                    case Symbol::GROUND:
                        break;
                    default:
                        assert(false);
                }
            }
        }
        return grid;
    }

    Pipe parse::parse_pipe(const char& c) {
        switch(static_cast<Symbol>(c)) {
            case Symbol::VERTICAL:
                return { Pipe::Direction::NORTH, Pipe::Direction::SOUTH };
            case Symbol::HORIZONTAL:
                return { Pipe::Direction::EAST, Pipe::Direction::WEST };
            case Symbol::NE_CORNER:
                return { Pipe::Direction::NORTH, Pipe::Direction::EAST };
            case Symbol::NW_CORNER:
                return { Pipe::Direction::NORTH, Pipe::Direction::WEST };
            case Symbol::SE_CORNER:
                return { Pipe::Direction::SOUTH, Pipe::Direction::EAST };
            case Symbol::SW_CORNER:
                return { Pipe::Direction::SOUTH, Pipe::Direction::WEST };
            default:
                assert(false);
                return {};
        }
    }
}
