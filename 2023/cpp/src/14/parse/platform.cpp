//
// Created by Conor Pappas on 1/4/24.
//

#include "platform.hpp"

#include "data_types/platform.hpp"

namespace dish::parse {
    data_types::Platform parse_platform(const std::vector<std::string>& rows) {
        data_types::Platform::container entities;
        for (const std::string& row : rows) {
            data_types::Platform::container::value_type entities_row;
            for (const char& c : row) {
                entities_row.push_back(static_cast<data_types::Entity>(c));
            }
            entities.push_back(entities_row);
        }
        return data_types::Platform(entities);
    }
}
