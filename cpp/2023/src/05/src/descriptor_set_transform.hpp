//
// Created by Conor Pappas on 12/13/23.
//

#ifndef DESCRIPTOR_SET_TRANSFORM_HPP
#define DESCRIPTOR_SET_TRANSFORM_HPP

#include <vector>

#include "descriptor_range_transform.hpp"

class DescriptorSetTransform {
public:
    DescriptorSetTransform(Category  domain_category, Category  image_category);

    void add(const DescriptorRangeTransform& transform);

    [[nodiscard]] Category domain_category() const;
    [[nodiscard]] Category image_category() const;

    [[nodiscard]] Descriptor operator()(const Descriptor& input) const;
    [[nodiscard]] DescriptorSet operator()(const DescriptorSet& input) const;

    friend ostream& operator<<(ostream& os, const DescriptorSetTransform& d);

private:
    Category _domain_category;
    Category _image_category;
    vector<DescriptorRangeTransform> transforms{};
};

#endif //DESCRIPTOR_SET_TRANSFORM_HPP
