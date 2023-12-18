//
// Created by Conor Pappas on 12/13/23.
//

#ifndef ALMANAC_HPP
#define ALMANAC_HPP

#include <iostream>
#include <unordered_map>

#include "descriptor_set.hpp"
#include "descriptor_set_transform.hpp"

using namespace std;

class Almanac {
public:
    std::unordered_map<Category, DescriptorSetTransform> transform_sets;

    DescriptorSet operator()(const DescriptorSet& input, const Category& target_category) const;

    friend ostream& operator<<(ostream& os, const Almanac& d);
};

#endif //ALMANAC_HPP
