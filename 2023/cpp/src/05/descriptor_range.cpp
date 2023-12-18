//
// Created by Conor Pappas on 12/13/23.
//

#include "descriptor_range.hpp"

#include <cassert>
#include <algorithm>

#include "descriptor.hpp"

using namespace std;

DescriptorRange::DescriptorRange(Descriptor start, const long size):
    _range_start(std::move(start)),
    _size(size) {
    assert(_size >= 0);
}

DescriptorRange DescriptorRange::empty(const Category& category) {
    return DescriptorRange({category, 0}, 0);
}

long DescriptorRange::start() const { return _range_start.value; }

long DescriptorRange::end() const { return _range_start.value + size() - 1; }

Descriptor DescriptorRange::start_descriptor() const { return _range_start; }

Descriptor DescriptorRange::end_descriptor() const { return {_range_start.category, end()}; }

Category DescriptorRange::category() const { return _range_start.category; }

bool DescriptorRange::empty() const { return size() == 0;}

long DescriptorRange::size() const { return _size; }

bool DescriptorRange::in_range(const Descriptor& input) const {
    assert(input.category == category());
    return input.value >= start() && input.value <= end();
}

std::tuple<DescriptorRange, DescriptorRange> DescriptorRange::split(const Descriptor& split_point) const {
    if (split_point.value < start()) {
        return {empty(category()), *this};
    }
    if (split_point.value > end()) {
        return {*this, empty(category())};
    }
    const long split_offset = split_point.value - start();
    const long left_size = split_offset + 1;
    const long right_size = size() - split_offset - 1;
    const DescriptorRange left = { {category(), start()}, left_size };
    const DescriptorRange right = { {category(), split_point.value + 1}, right_size };
    assert(left.size() + right.size() == size());
    return {left, right};
}

DescriptorRange DescriptorRange::intersect(const DescriptorRange& other) const {
    assert(other.category() == category());
    const long range_start = max(start(), other.start());
    const long range_end = min(end(), other.end());
    const long range_size = range_end - range_start + 1;
    if (range_size <= 0) {
        return DescriptorRange::empty(category());
    } else {
        return DescriptorRange({category(), range_start}, range_size);
    }
}

std::ostream& operator<<(std::ostream& os, const DescriptorRange& d) {
    return os << d.category() << "(" << d.start() << "-" << d.end() << ")";
}
