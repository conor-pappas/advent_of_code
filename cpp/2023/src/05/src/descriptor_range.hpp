//
// Created by Conor Pappas on 12/13/23.
//

#ifndef DESCRIPTOR_RANGE_H
#define DESCRIPTOR_RANGE_H

#include <ostream>

#include "descriptor.hpp"

class DescriptorRange {
public:
    DescriptorRange(Descriptor  start, long size);

    [[nodiscard]] static DescriptorRange empty(const Category& category);

    // TODO: start/end should be Descriptor
    [[nodiscard]] long start() const;
    [[nodiscard]] long end() const;
    [[nodiscard]] Descriptor start_descriptor() const;
    [[nodiscard]] Descriptor end_descriptor() const;
    [[nodiscard]] Category category() const;

    [[nodiscard]] bool empty() const;
    [[nodiscard]] long size() const;

    [[nodiscard]] bool in_range(const Descriptor& input) const;

    [[nodiscard]] std::tuple<DescriptorRange, DescriptorRange> split(const Descriptor& split_point) const;
    [[nodiscard]] DescriptorRange intersect(const DescriptorRange& other) const;

    friend std::ostream& operator<<(std::ostream& os, const DescriptorRange& d);
private:
    Descriptor _range_start;
    long _size;
};

#endif //DESCRIPTOR_RANGE_H
