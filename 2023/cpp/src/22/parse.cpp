//
// Created by Conor Pappas on 1/17/24.
//

#include "parse.hpp"

#include <cassert>
#include <regex>

namespace disintegrate::parse {

    const std::regex BLOCK_REGEX(R"((\d+),(\d+),(\d+)~(\d+),(\d+),(\d+))");
    data_types::Block parse_block(const std::string& str) {
        std::smatch match;
        std::regex_match(str, match, BLOCK_REGEX);
        if(!std::regex_match(str, match, BLOCK_REGEX)) {
            throw std::runtime_error("Invalid workflow: " + str);
        }
        data_types::Block result;
        result.x.min = std::stoi(match[1]);
        result.y.min = std::stoi(match[2]);
        result.z.min = std::stoi(match[3]);
        result.x.max = std::stoi(match[4]);
        result.y.max = std::stoi(match[5]);
        result.z.max = std::stoi(match[6]);

        const auto x_equal = result.x.min == result.x.max;
        const auto y_equal = result.y.min == result.y.max;
        const auto z_equal = result.z.min == result.z.max;
        assert((x_equal && y_equal) || (x_equal && z_equal) || (y_equal && z_equal));

        return result;
    }

    std::vector<data_types::Block> parse_block(const std::vector<std::string>& strs) {
        std::vector<data_types::Block> result;
        result.reserve(strs.size());
        int start_label = 0;
        for(const auto& str : strs) {
            auto block = parse_block(str);
            block.label = start_label++;
            result.push_back(block);
        }
        return result;
    }
}
