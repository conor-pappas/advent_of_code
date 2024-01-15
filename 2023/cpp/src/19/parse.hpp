//
// Created by Conor Pappas on 1/15/24.
//

#pragma once

#include <string>

#include "data_types/plan.hpp"

namespace avalanche::parse {
    using PartCollection = std::vector<data_types::Part>;
    data_types::attribute_type parse_attribute_type(char);
    data_types::Part parse_part(const std::string&);
    data_types::Rule parse_rule(const std::string&);
    data_types::Workflow parse_workflow(const std::string&);
    std::tuple<data_types::Plan, PartCollection> parse_input(const std::vector<std::string>&);
}
