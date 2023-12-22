//
// Created by Conor Pappas on 12/22/23.
//

#include "string.hpp"

#include <vector>
#include <string>

namespace support {
    // TODO: split_view giving me trouble. Would like to make this better.
    std::vector<std::string> split(const std::string& str, const char& delim) {
        std::vector<std::string> strings;
        std::string current_string;
        auto push = [&]() {
            if(!current_string.empty()) { strings.push_back(current_string); }
            current_string.clear();
        };
        for (const auto& c : str) {
            if (c == delim) { push(); }
            else {  current_string.push_back(c); }
        }
        push();
        return strings;
    }

    std::string join(const std::vector<std::string>& strings, const std::string& delim) {
        std::string result;
        join(strings, delim, result);
        return result;
    }

    void join(const std::vector<std::string>& strings, const std::string& delim, std::string& result) {
        for (size_t i=0; i < strings.size(); i++) {
            result += strings[i];
            if (i != strings.size() - 1) {
                result += delim;
            }
        }
    }
}
