//
// Created by Conor Pappas on 12/22/23.
//

#pragma once
#include <regex>
#include <vector>

#include "record.hpp"
#include "pattern.hpp"

namespace spring::parse {
    const std::regex RECORD_REGEX = std::regex(R"(([#.?]+)\s*([\d,]+))");

    struct Row {
        Record record;
        Pattern pattern;
    };

    std::vector<Row> parse_rows(const std::vector<std::string>&);
    Pattern parse_pattern(const std::string&);
    long parse_long(const std::string&);
};
