//
// Created by Conor Pappas on 1/6/24.
//

#include "hasher.hpp"

#include <iostream>

namespace library::data_types {
    unsigned char Hasher::operator()(const std::string& str) const {
        static_assert(CHAR_BIT == 8);
        unsigned char hash = 0;
        for(const unsigned char c : str) {
            hash += c;
            hash *= 17;
        }
        return hash;
    }
}
