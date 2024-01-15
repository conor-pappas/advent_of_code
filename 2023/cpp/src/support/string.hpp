//
// Created by Conor Pappas on 12/22/23.
//

#pragma once

#include <string>
#include <vector>

#include "range_concepts.hpp"

namespace support {
    // TODO: provide a version with a string delim
    [[nodiscard]] std::vector<std::string> split(const std::string&, const char&);

    template<input_range_of<std::string> Range>
    [[nodiscard]] std::string join(const Range&, const std::string&);

    template<input_range_of<std::string> Range>
    void join(const Range&, const std::string&, std::string&);

    template<input_range_of<std::string> Range>
    std::string join(const Range& range, const std::string& delim) {
        std::string result;
        join(range, delim, result);
        return result;
    }

    template<input_range_of<std::string> Range>
    void join(const Range& range, const std::string& delim, std::string& result) {
        using namespace std::ranges;
        for (auto itr = std::begin(range); itr < std::end(range); ++itr) {
            result += *itr;
            if (itr + 1 != std::end(range)) {
                result += delim;
            }
        }
    }
};
