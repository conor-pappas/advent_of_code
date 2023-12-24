//
// Created by Conor Pappas on 12/22/23.
//

#pragma once
#include <regex>
#include <vector>

#include "data_types/record.hpp"
#include "data_types/pattern.hpp"

namespace spring::parse {
    const std::regex RECORD_REGEX = std::regex(R"(([#.?]+)\s*([\d,]+))");

    struct Row {
        data_types::Record record;
        data_types::Pattern pattern;
    };

    std::vector<Row> parse_rows(const std::vector<std::string>&);
    data_types::Pattern parse_pattern(const std::string&);
    long parse_long(const std::string&);
};
