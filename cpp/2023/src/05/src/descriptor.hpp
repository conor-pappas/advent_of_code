//
// Created by Conor Pappas on 12/13/23.
//

#ifndef DESCRIPTOR_H
#define DESCRIPTOR_H

#include <string>

using Category = std::string;

// TODO: Make generic type for long
struct Descriptor {
    Category category;
    long value;

    Descriptor operator+(const long&) const;
    Descriptor operator-(const long&) const;

    friend std::ostream& operator<<(std::ostream& os, const Descriptor& d);
};

#endif //DESCRIPTOR_H
