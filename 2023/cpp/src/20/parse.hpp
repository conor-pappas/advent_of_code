//
// Created by Conor Pappas on 1/15/24.
//

#pragma once

#include <vector>

#include "data_types/network.hpp"

namespace pulse::parse {
    struct Rule {
        data_types::label origin;
        data_types::ModuleType module_type;
        std::vector<data_types::label> destinations;
    };

    Rule parse_rule(const std::string&);
    data_types::Network parse_network(const std::vector<std::string>&);
};
