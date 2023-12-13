//
// Created by Conor Pappas on 12/13/23.
//

#include <ostream>

#include "descriptor.hpp"

std::ostream& operator<<(std::ostream& os, const Descriptor& d) {
    return os << d.category << ":" << d.value;
};

Descriptor Descriptor::operator+(const long& rhs) const {
    return { category, value + rhs };
}

Descriptor Descriptor::operator-(const long& rhs) const {
    return { category, value - rhs };
}
