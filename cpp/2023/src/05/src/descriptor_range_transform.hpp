//
// Created by Conor Pappas on 12/13/23.
//

#ifndef DESCRIPTOR_RANGE_TRANSFORM_HPP
#define DESCRIPTOR_RANGE_TRANSFORM_HPP

#include "transform_result.hpp"

class DescriptorRangeTransform {
public:
    DescriptorRange domain_range;
    DescriptorRange image_range;

    DescriptorRangeTransform(const DescriptorRange& domain_range, const DescriptorRange& image_range);

    [[nodiscard]] Category domain_category() const;
    [[nodiscard]] Category image_category() const;

    [[nodiscard]] long offset() const;

    [[nodiscard]] bool in_range(const Descriptor& input) const;

    Descriptor operator()(const Descriptor& input) const;
    void operator()(TransformResult& input) const;

    friend ostream& operator<<(ostream& os, const DescriptorRangeTransform& d);
};

#endif //DESCRIPTOR_RANGE_TRANSFORM_HPP
