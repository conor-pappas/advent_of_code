//
// Created by Conor Pappas on 1/6/24.
//

#include "hash.hpp"

#include <iostream>

namespace library {
    unsigned char hash(const std::string& str) {
        unsigned char hash = 0;
        for(const unsigned char c : str) {
            hash += c;
            hash *= 17;
        }
        return hash;
    }
}
