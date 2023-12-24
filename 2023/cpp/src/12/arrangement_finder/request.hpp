//
// Created by Conor Pappas on 12/23/23.
//

#pragma once

#include "data_types/pattern.hpp"
#include "data_types/record.hpp"

namespace spring::arrangement_finder {
    struct Request {
        struct Hasher;
        data_types::Record record {};
        data_types::Pattern pattern {};

        friend bool operator==(const Request&, const Request&);
        friend std::ostream& operator<<(std::ostream&, const Request&);
    };

    struct Request::Hasher {
        size_t operator()(const Request& request) const;
    };
}
