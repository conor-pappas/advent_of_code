//
// Created by Conor Pappas on 12/22/23.
//

#pragma once

#include <string>
#include <vector>

namespace support {
    // TODO: provide a version with a string delim
    [[nodiscard]] std::vector<std::string> split(const std::string&, const char&);
    [[nodiscard]] std::string join(const std::vector<std::string>&, const std::string&);
    void join(const std::vector<std::string>&, const std::string&, std::string&);
};
