//
// Created by Conor Pappas on 12/18/23.
//

#include "parse.hpp"

std::vector<oasis::History> oasis::parse::parse_input(const std::vector<std::string>& lines) {
    std::vector<History> histories;
    histories.reserve(lines.size());
    for (const auto& line : lines) {
        histories.push_back(parse_history(line));
    }
    return histories;
}

oasis::History oasis::parse::parse_history(const std::string& line) {
    History history;
    for(const auto& number : support::split(line, ' ')) {
        history.add(stol(number));
    }
    return history;
}
