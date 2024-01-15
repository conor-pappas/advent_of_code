//
// Created by Conor Pappas on 1/15/24.
//

#include "workflow.hpp"

#include "rule.hpp"

namespace avalanche::data_types {
    label follow_workflow(const Workflow& workflow, const Part& part) {
        for (const auto& rule : workflow.rules) {
            auto next_label = follow(rule, part);
            if(next_label.has_value()) return next_label.value();
        }
        return workflow.default_label;
    }
}
