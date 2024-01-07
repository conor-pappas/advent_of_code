//
// Created by Conor Pappas on 1/6/24.
//

#include "step.hpp"

#include <cassert>
#include <regex>

#include <string.hpp>


namespace library::parse {
    const std::regex step_regex(R"((\w+)((-)|(=(\d+))))");
    data_types::Step parse_step(const std::string& str) {
        std::smatch match;
        if(!std::regex_match(str, match, step_regex)) {
            throw std::runtime_error("Invalid step: " + str);
        }
        data_types::Step step;
        step.label = match[1];
        if(match[3].matched) {
            step.op = data_types::Operator::REMOVE;
        } else if(match[4].matched) {
            step.op = data_types::Operator::SET;
            step.lense = std::stoi(match[5]);
        } else {
            throw std::runtime_error("Invalid step: " + str);
        }
        return step;
    }

    std::vector<data_types::Step> parse_steps(const std::string& str) {
        std::vector<data_types::Step> steps;
        for(const auto& step_str : support::split(str, ',')) {
            steps.push_back(parse_step(step_str));
        }
        return steps;
    }
}
