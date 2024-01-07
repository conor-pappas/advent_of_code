//
// Created by Conor Pappas on 1/6/24.
//

#pragma once

#include <string>

namespace library::data_types {

    using Label = std::string;
    using Lense = unsigned char;

    enum Operator {
        REMOVE = '-',
        SET = '='
    };

    struct Step {
        Label label;
        Operator op;
        Lense lense { 0 };
    };
}
