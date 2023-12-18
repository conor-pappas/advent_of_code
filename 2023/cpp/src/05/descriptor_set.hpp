//
// Created by Conor Pappas on 12/13/23.
//

#ifndef DESCRIPTOR_SET_H
#define DESCRIPTOR_SET_H

#include <set>

#include "descriptor_range.hpp"

using namespace std;

class DescriptorSet {
    // NOTE:
    // This compare assumes the two ranges don't intersect
    struct DescriptorRangeCompare {
        bool operator() (const DescriptorRange& lhs, const DescriptorRange& rhs) const;
    };
public:
    using RangeSet = set<DescriptorRange, DescriptorRangeCompare>;

    explicit DescriptorSet(Category);
    explicit DescriptorSet(const DescriptorRange& single_range);

    [[nodiscard]] Category category() const;
    [[nodiscard]] RangeSet ranges() const;

    [[nodiscard]] long size() const;
    [[nodiscard]] bool empty() const;

    void insert(const DescriptorRange&);
    void insert(const DescriptorSet&);
    void erase(const DescriptorRange&);
    void erase(const DescriptorSet&);

    friend ostream& operator<<(ostream&, const DescriptorSet&);

    Category _category;
    RangeSet _ranges;
};

#endif //DESCRIPTOR_SET_H
