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

    PartRangeResultContainer follow_workflow(const Workflow& workflow, PartRange orig) {
        auto part_range = orig;
        PartRangeResultContainer results;
        for (const auto& rule : workflow.rules) {
            auto followed = follows(rule, part_range);
            part_range = skips(rule, part_range);
            results.emplace_back(rule.label, followed);
        }
        results.emplace_back(workflow.default_label, part_range);
        return results;
    }
}
