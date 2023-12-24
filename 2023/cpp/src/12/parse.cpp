//
// Created by Conor Pappas on 12/22/23.
//

#include "parse.hpp"

#include <cassert>
#include <regex>
#include <iostream>
#include <ranges>

#include "string.hpp"

namespace spring::parse {
    std::vector<Row> parse_rows(const std::vector<std::string>& lines) {
        std::vector<Row> rows;
        for (const auto& line : lines) {
            std::smatch match;
            std::regex_match(line, match, RECORD_REGEX);
            assert(match.size() == 3);
            auto conditions = std::string_view(match[1].str())
                | std::views::transform(data_types::Record::char_to_condition);
            const data_types::Record record { conditions.begin(), conditions.end() };
            rows.push_back({record, parse_pattern(match[2])});
        }
        return rows;
    }

    data_types::Pattern parse_pattern(const std::string& str) {
        const auto numbers = support::split(str, ',')
            | std::views::transform(parse_long);
        return {numbers.begin(), numbers.end()};
    }

    long parse_long(const std::string& str) {
        return std::stol(str);
    }
}
