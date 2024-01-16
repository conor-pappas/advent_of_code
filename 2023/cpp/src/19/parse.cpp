//
// Created by Conor Pappas on 1/15/24.
//

#include "parse.hpp"

#include <regex>

#include <string.hpp>

namespace avalanche::parse {

    const std::regex PART_REGEX(R"(\{x=(\d+),m=(\d+),a=(\d+),s=(\d+)\})");
    data_types::Part parse_part(const std::string& part) {
        data_types::Part result;
        std::smatch match;
        if(!std::regex_match(part, match, PART_REGEX)) {
            throw std::runtime_error("Invalid part: " + part);
        }
        result.x = std::stoi(match[1]);
        result.m = std::stoi(match[2]);
        result.a = std::stoi(match[3]);
        result.s = std::stoi(match[4]);
        return result;
    }

    const std::regex RULE_REGEX(R"((\w+)([<>])(\d+):(\w+))");
    data_types::Rule parse_rule(const std::string& str) {
        data_types::Rule result;
        std::smatch match;
        if(!std::regex_match(str, match, RULE_REGEX)) {
            throw std::runtime_error("Invalid step: " + str);
        }
        result.attribute = static_cast<data_types::AttributeType>(match[1].str()[0]);
        result.compare = static_cast<data_types::Compare>(match[2].str()[0]);
        result.value = std::stoi(match[3]);
        result.label = match[4];
        return result;
    }

    const std::regex WORKFLOW_REGEX(R"((\w+)\{(.*)\})");
    data_types::Workflow parse_workflow(const std::string& str) {
        data_types::Workflow result;
        std::smatch match;
        if(!std::regex_match(str, match, WORKFLOW_REGEX)) {
            throw std::runtime_error("Invalid workflow: " + str);
        }
        result.name = match[1];
        const auto rule_strings = support::split(match[2], ',');
        for(auto itr = rule_strings.begin(); itr != rule_strings.end(); ++itr) {
            if(itr == --rule_strings.end()) {
                result.default_label = *itr;
            } else {
                result.rules.push_back(parse_rule(*itr));
            }
        }
        return result;
    }

    std::tuple<data_types::Plan, PartCollection> parse_input(const std::vector<std::string>& strs) {
        data_types::Plan plan;
        PartCollection parts;
        auto itr = strs.begin();
        for(; !itr->empty(); ++itr) {
            const auto workflow = parse_workflow(*itr);
            plan.add_workflow(workflow.name, workflow);
        }
        ++itr;
        for(; itr != strs.end(); ++itr) {
            parts.push_back(parse_part(*itr));
        }
        return {plan, parts};
    }
}
