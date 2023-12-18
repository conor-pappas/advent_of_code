//
// Created by Conor Pappas on 12/13/23.
//

#include "descriptor_range_transform.hpp"

#include <cassert>

DescriptorRangeTransform::DescriptorRangeTransform(const DescriptorRange& domain_range,
                                                   const DescriptorRange& image_range):
    domain_range(domain_range),
    image_range(image_range) {
    assert(domain_range.size() == image_range.size());
}

Category DescriptorRangeTransform::domain_category() const { return domain_range.category(); }

Category DescriptorRangeTransform::image_category() const { return image_range.category(); }

long DescriptorRangeTransform::offset() const {
    return image_range.start() - domain_range.start();
}

bool DescriptorRangeTransform::in_range(const Descriptor& input) const {
    return domain_range.in_range(input);
}

Descriptor DescriptorRangeTransform::operator()(const Descriptor& input) const {
    assert(in_range(input));
    return Descriptor{image_range.category(), input.value + offset()};
}

void DescriptorRangeTransform::operator()(TransformResult& input) const {
    assert(input.domain_category() == domain_category());
    assert(input.image_category() == image_category());

    input.map_domain([&](const DescriptorRange& range) {
        auto [bottom, rest] = range.split(domain_range.start_descriptor() - 1);
        auto [middle, top] = rest.split(domain_range.end_descriptor());

        DescriptorSet unresolved_domain(domain_category());
        DescriptorSet result(image_category());
        unresolved_domain.insert(bottom);
        unresolved_domain.insert(top);
        if (!middle.empty()) {
            result.insert({(*this)(middle.start_descriptor()), middle.size()});
        }
        return tuple{result, unresolved_domain};
    });
}

ostream& operator<<(ostream& os, const DescriptorRangeTransform& d) {
    return os << d.domain_range << " -> " << d.image_range << "[offset: " << d.offset() << "]";
}
