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
}
