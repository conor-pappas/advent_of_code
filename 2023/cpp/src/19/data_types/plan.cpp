//
// Created by Conor Pappas on 1/15/24.
//

#include "plan.hpp"

#include "label.hpp"

namespace avalanche::data_types {
    void Plan::add_workflow(const label& label, const Workflow& workflow) {
        m_workflows[label] = workflow;
    }

    const Workflow& Plan::lookup_workflow(const label& label) const {
        return m_workflows.at(label);
    }

    bool test(const Plan& plan, const Part& part) {
        label next_label = START;
        while(next_label != ACCEPT && next_label != REJECT) {
            const auto& workflow = plan.lookup_workflow(next_label);
            next_label = follow_workflow(workflow, part);
        }
        return next_label == ACCEPT;
    }

    PartRangeAcceptance test(const Plan& plan, const PartRange& part_range) {
        PartRangeAcceptance results;
        PartRangeResultContainer unresolved = {{START, part_range}};
        while(!unresolved.empty()) {
            auto [label, part_range] = unresolved.back();
            unresolved.pop_back();

            if(label == ACCEPT) {
                results.accept.push_back(part_range);
            } else if(label == REJECT) {
                results.reject.push_back(part_range);
            } else {
                const auto& workflow = plan.lookup_workflow(label);
                auto workflow_results = follow_workflow(workflow, part_range);
                unresolved.insert(unresolved.begin(), workflow_results.begin(), workflow_results.end());
            }
        }
        return results;
    }
}
