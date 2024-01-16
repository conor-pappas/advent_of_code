//
// Created by Conor Pappas on 1/15/24.
//

#include "parse.hpp"

#include <cassert>
#include <regex>

#include <string.hpp>

namespace pulse::parse {

    const std::regex RULE_REGEX(R"(([%&]?)(\w+)\s*->\s*([\w\s,]+))");
    Rule parse_rule(const std::string& str) {
        std::smatch match;
        std::regex_match(str, match, RULE_REGEX);
        if(!std::regex_match(str, match, RULE_REGEX)) {
            throw std::runtime_error("Invalid workflow: " + str);
        }
        Rule result;
        result.origin = match[2];
        if(match[1] == "%") {
            result.module_type = data_types::ModuleType::FlipFlop;
        } else if(match[1] == "&") {
            result.module_type = data_types::ModuleType::Conjunction;
        } else {
            assert(result.origin == data_types::BROADCASTER);
            result.module_type = data_types::ModuleType::Broadcast;
        }

        auto destination_strings = support::split(match[3], ',');
        for(auto& destination_string : destination_strings) {
            result.destinations.push_back(support::trim(destination_string));
        }
        return result;
    }

    data_types::Network parse_network(const std::vector<std::string>& strs) {
        std::vector<Rule> rules;
        data_types::Network network;
        for(const auto& str : strs) {
            const auto rule = parse_rule(str);
            rules.push_back(rule);
            register_module(network, rule.origin, rule.module_type);
        }
        for(const auto& rule : rules) {
            link_modules(network, rule.origin, rule.destinations);
        }
        return network;
    }
}
