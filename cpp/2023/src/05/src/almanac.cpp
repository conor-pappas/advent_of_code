//
// Created by Conor Pappas on 12/13/23.
//

#include "almanac.hpp"

DescriptorSet Almanac::operator()(const DescriptorSet& input, const Category& target_category) const {
    DescriptorSet result = input;
    while (result.category() != target_category) {
        result = transform_sets.at(result.category())(result);
    }
    return result;
}

ostream& operator<<(ostream& os, const Almanac& d) {
    os << "Almanac: " << endl;
    for (const auto& [category, transform] : d.transform_sets) {
        os << transform << endl;
    }
    return os;
}
