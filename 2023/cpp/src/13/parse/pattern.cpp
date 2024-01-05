//
// Created by Conor Pappas on 12/23/23.
//

#include "pattern.hpp"

#include <vector>

#include "data_types/pattern.hpp"
#include "data_types/entity.hpp"

namespace incidence::parse {

    // TODO: This is the same as string.hpp/join(). Could make a split_view for this type of thing.
    // (One day c++23 :[ )
    std::vector<data_types::Pattern> parse_patterns(const std::vector<std::string>& rows) {
        std::vector<data_types::Pattern> patterns;
        std::vector<std::string> pattern_rows;
        for (const std::string& row : rows) {
            if (row.empty()) {
                patterns.push_back(parse_pattern(pattern_rows));
                pattern_rows.clear();
            } else {
                pattern_rows.push_back(row);
            }
        }
        patterns.push_back(parse_pattern(pattern_rows));
        return patterns;
    }

    data_types::Pattern parse_pattern(const std::vector<std::string>& rows) {
        support::Grid<data_types::Entity,2> entities;
        for (const std::string& row : rows) {
            support::Grid<data_types::Entity,1> entities_row;
            for (const char& c : row) {
                entities_row.push_back(static_cast<data_types::Entity>(c));
            }
             entities.push_back(entities_row);
        }
        return data_types::Pattern(entities);
    }
}
