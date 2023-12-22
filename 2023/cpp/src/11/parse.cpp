//
// Created by Conor Pappas on 12/22/23.
//

#include "parse.hpp"

#include <vector>
#include <string>

namespace cosmic {
    Space parse::parse_space(const std::vector<std::string>& lines) {
        Space space;
        space.set_height(lines.size());
        space.set_width(lines[0].size());

        for(Space::coord_type y=0; y<space.get_height(); ++y) {
            for(Space::coord_type x=0; x<space.get_width(); ++x) {
                if(static_cast<Symbol>(lines[y][x]) == Symbol::GALAXY) {
                    space.add_galaxy(Space::Galaxy{x, y});
                }
            }
        }
        return space;
    }
}
