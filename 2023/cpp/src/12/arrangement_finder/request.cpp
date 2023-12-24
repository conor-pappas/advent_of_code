//
// Created by Conor Pappas on 12/23/23.
//

#include "request.hpp"

#include <ostream>

#include "data_types/record.hpp"
#include "data_types/pattern.hpp"

namespace spring::arrangement_finder {
    size_t Request::Hasher::operator()(const Request& request) const {
        return data_types::Record::Hasher{}(request.record)
            ^ data_types::Pattern::Hasher{}(request.pattern);
    }

    bool operator==(const Request& a, const Request& b) {
        return a.record == b.record && a.pattern == b.pattern;
    }

    std::ostream& operator<<(std::ostream& os, const Request& request) {
        return os << request.record << "|" << request.pattern;
    }
}
