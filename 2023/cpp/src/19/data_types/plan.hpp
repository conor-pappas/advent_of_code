//
// Created by Conor Pappas on 1/15/24.
//

#pragma once

#include <unordered_map>

#include "label.hpp"
#include "workflow.hpp"
#include "parts.hpp"

namespace avalanche::data_types {

    class Plan {
    public:
        void add_workflow(const label&, const Workflow&);
        [[nodiscard]] const Workflow& lookup_workflow(const label&) const;
    private:
        std::unordered_map<label, Workflow> m_workflows {};
    };

    using PartRangeList = std::vector<PartRange>;
    struct PartRangeAcceptance {
        PartRangeList accept {};
        PartRangeList reject {};
    };

    bool test(const Plan&, const Part&);
    PartRangeAcceptance test(const Plan&, const PartRange&);
}
