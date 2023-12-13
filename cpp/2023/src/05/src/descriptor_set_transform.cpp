//
// Created by Conor Pappas on 12/13/23.
//

#include "descriptor_set_transform.hpp"

#include <cassert>
#include <ranges>

void DescriptorSetTransform::add(const DescriptorRangeTransform& transform) {
    assert(transform.domain_range.category() == _domain_category);
    assert(transform.image_range.category() == _image_category);
    transforms.push_back(transform);
}

Category DescriptorSetTransform::domain_category() const { return _domain_category; }

Category DescriptorSetTransform::image_category() const { return _image_category; }

Descriptor DescriptorSetTransform::operator()(const Descriptor& input) const {
    auto result = transforms
                  | views::filter([&](const auto& transform) { return transform.in_range(input); })
                  | views::transform([&](const auto& transform) { return transform(input); });
    if (result.empty()) {
        return {_image_category, input.value};
    } else {
        assert(distance(result.begin(), result.end()) == 1);
        return *result.begin();
    }
}

DescriptorSet DescriptorSetTransform::operator()(const DescriptorSet& input) const {
    assert(input.category() == domain_category());

    TransformResult result(input, _image_category);
    for (const auto& transform : transforms) { transform(result); }
    result.resolve_domain();
    return result.image;
}

DescriptorSetTransform::DescriptorSetTransform(Category domain_category, Category image_category):
    _domain_category(std::move(domain_category)),
    _image_category(std::move(image_category)) {}

ostream& operator<<(ostream& os, const DescriptorSetTransform& d) {
    for (const auto& transform : d.transforms) {
        os << transform << endl;
    }
    return os;
}
