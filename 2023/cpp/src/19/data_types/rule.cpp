//
// Created by Conor Pappas on 1/15/24.
//

#include "rule.hpp"

namespace avalanche::data_types {
    bool matches(const Rule& rule, const Part& part) {
        const rating target_rating = part.*rule.attribute;
        switch(rule.compare) {
            case Compare::Greater:
                return target_rating > rule.value;
            case Compare::Less:
                return target_rating < rule.value;
            default:
                throw std::runtime_error("Invalid Compare");
        }
    }

    std::optional<label> follow(const Rule& rule, const Part& part) {
        if(matches(rule, part)) return rule.label;
        return {};
    }
}
