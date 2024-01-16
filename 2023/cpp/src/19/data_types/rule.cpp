//
// Created by Conor Pappas on 1/15/24.
//

#include "rule.hpp"

namespace avalanche::data_types {
    bool matches(const Rule& rule, const Part& part) {
        const rating target_rating = read_attr(part, rule.attribute);
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

    PartRange follows(const Rule& rule, PartRange part_range) {
        auto& [min, max] = read_attr(part_range, rule.attribute);
        switch(rule.compare) {
            case Compare::Greater:
                min = std::max(min, rule.value + 1);
                break;
            case Compare::Less:
                max = std::min(max, rule.value - 1);
                break;
            default:
                throw std::runtime_error("Invalid Compare");
        }
        return part_range;
    }

    PartRange skips(const Rule& rule, PartRange part_range) {
        auto& [min, max] = read_attr(part_range, rule.attribute);
        switch(rule.compare) {
            case Compare::Greater:
                max = std::min(max, rule.value);
                break;
            case Compare::Less:
                min = std::max(min, rule.value);
                break;
            default:
                throw std::runtime_error("Invalid Compare");
        }
        return part_range;
    }
}
