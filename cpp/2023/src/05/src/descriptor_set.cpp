//
// Created by Conor Pappas on 12/13/23.
//

#include "descriptor_set.hpp"

#include <cassert>
#include <numeric>
#include <set>

#include "descriptor.hpp"
#include "descriptor_range.hpp"

using namespace std;

DescriptorSet::DescriptorSet(Category category):
    _category(std::move(category)) {}

DescriptorSet::DescriptorSet(const DescriptorRange& single_range):
    _category(single_range.category()) {
        insert(single_range);
}

Category DescriptorSet::category() const { return _category; }

DescriptorSet::RangeSet DescriptorSet::ranges() const { return _ranges; }

long DescriptorSet::size() const {
    return accumulate(_ranges.begin(), _ranges.end(), 0, [](long sum, const DescriptorRange& range) {
        return sum + range.size();
    });
}

bool DescriptorSet::empty() const { return _ranges.empty(); }

void DescriptorSet::insert(const DescriptorRange& other) {
    assert(other.category() == _category);
    if(other.empty()) { return; }
    // TODO: assert that other is not a subset of any existing range (requires actual interval tree)
    _ranges.insert(other);
}

void DescriptorSet::insert(const DescriptorSet& other) {
    assert(other.category() == _category);
    _ranges.insert(other._ranges.begin(), other._ranges.end());
}

void DescriptorSet::erase(const DescriptorRange& other) {
    assert(other.category() == _category);
    _ranges.erase(other);
}

void DescriptorSet::erase(const DescriptorSet& other) {
    assert(other.category() == _category);
    _ranges.erase(other._ranges.begin(), other._ranges.end());
}

ostream& operator<<(ostream& os, const DescriptorSet& d) {
    os << d.category() << "[" << d.size() << "]: ";
    for (const auto& range : d.ranges()) {
        os << range << " ";
    }
    return os;
}

bool DescriptorSet::DescriptorRangeCompare::operator()(const DescriptorRange& lhs, const DescriptorRange& rhs) const {
    return lhs.end() < rhs.start();
}
