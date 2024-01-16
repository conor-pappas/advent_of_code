//
// Created by Conor Pappas on 1/15/24.
//

#pragma once

#include <vector>

#include "rule.hpp"
#include "label.hpp"
#include "parts.hpp"

namespace avalanche::data_types {

    using RuleContainer = std::vector<Rule>;
    struct Workflow {
        RuleContainer rules;
        label name;
        label default_label;
    };

    using PartRangeResult = std::tuple<label, PartRange>;
    using PartRangeResultContainer = std::vector<PartRangeResult>;

    label follow_workflow(const Workflow&, const Part&);
    PartRangeResultContainer follow_workflow(const Workflow&, PartRange);
}
