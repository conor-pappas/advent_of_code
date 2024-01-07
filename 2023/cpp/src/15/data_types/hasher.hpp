//
// Created by Conor Pappas on 1/6/24.
//

#pragma once

#include <string>

namespace library::data_types {
    struct Hasher {
        unsigned char operator()(const std::string& str) const;
    };
}
