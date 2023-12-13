//
// Created by Conor Pappas on 12/13/23.
//

#include "transform_result.hpp"

#include <utility>

TransformResult::TransformResult(DescriptorSet unresolved_domain, DescriptorSet image):
    unresolved_domain(std::move(unresolved_domain)),
    image(std::move(image)) {
}

TransformResult::TransformResult(DescriptorSet unresolved_domain, Category image_category):
    unresolved_domain(std::move(unresolved_domain)),
    image(std::move(image_category)) {
}

Category TransformResult::domain_category() const { return unresolved_domain.category(); }

Category TransformResult::image_category() const { return image.category(); }

void TransformResult::map_domain(const transform_func& func) {
    DescriptorSet new_unresolved_domain(domain_category());

    for (const auto& range : unresolved_domain.ranges()) {
        auto [mapped, unresolved] = func(range);
        image.insert(mapped);
        new_unresolved_domain.insert(unresolved);
    }
    unresolved_domain = new_unresolved_domain;
}

void TransformResult::resolve_domain() {
    for (const auto& range : unresolved_domain.ranges()) {
        image.insert({{image_category(), range.start()}, range.size()});
    }
}
