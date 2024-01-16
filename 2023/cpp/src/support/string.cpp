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

    std::string& ltrim(std::string& str, const char* whitespace) {
        str.erase(0, str.find_first_not_of(whitespace));
        return str;
    }

    std::string& rtrim(std::string& str, const char* whitespace) {
        str.erase(str.find_last_not_of(whitespace) + 1);
        return str;
    }

    std::string& trim(std::string& str, const char* whitespace) {
        return ltrim(rtrim(str, whitespace), whitespace);
    }

    std::string ltrim_copy(std::string str, const char* whitespace) {
        return std::move(ltrim(str, whitespace));
    }

    std::string rtrim_copy(std::string str, const char* whitespace) {
        return std::move(rtrim(str, whitespace));
    }

    std::string trim_copy(std::string str, const char* whitespace) {
        return std::move(trim(str, whitespace));
    }
}
