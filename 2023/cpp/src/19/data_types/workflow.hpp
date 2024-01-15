//
// Created by Conor Pappas on 1/15/24.
//

#pragma once

#include <vector>

#include "rule.hpp"
#include "label.hpp"

namespace avalanche::data_types {

    using RuleContainer = std::vector<Rule>;
    struct Workflow {
        RuleContainer rules;
        label name;
        label default_label;
    };

    label follow_workflow(const Workflow&, const Part&);
}
