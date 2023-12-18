//
// Created by Conor Pappas on 12/13/23.
//

#ifndef TRANSFORM_RESULT_H
#define TRANSFORM_RESULT_H

#include <functional>
#include "descriptor_set.hpp"

class TransformResult {
public:
    DescriptorSet unresolved_domain;
    DescriptorSet image;

    TransformResult(DescriptorSet unresolved_domain, DescriptorSet image);
    TransformResult(DescriptorSet unresolved_domain, Category image_category);

    [[nodiscard]] Category domain_category() const;
    [[nodiscard]] Category image_category() const;

    using transform_func = std::function<tuple<DescriptorSet, DescriptorSet>(const DescriptorRange&)>;
    void map_domain(const transform_func&);
    void resolve_domain();
};

#endif //TRANSFORM_RESULT_H
