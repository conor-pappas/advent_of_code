//
// Created by Conor Pappas on 12/17/23.
//

#include "input.hpp"

namespace support {
    std::filesystem::path input_dir() {
        // TODO: figure out loading from ENV variables
        return INPUT_DIR;
        // std::filesystem::path dir = std::getenv(INPUT_DIR_ENV_VAR);
        // if(dir.empty()) throw std::runtime_error("No AOC_INPUT_DIR environment variable set");
        // return dir;
    }

    std::vector<std::string> read_file(const std::filesystem::path& file_path) {
        std::ifstream file(file_path);

        if(!file) throw std::runtime_error("Could not open file");

        std::vector<std::string> lines;
        std::string line;
        while (getline(file, line)) {
            lines.push_back(line);
        }
        return lines;
    }

    std::vector<std::string> read_input(const std::string& day, int argc, const char** argv) {
        const std::string file_name = argc > 1 ? argv[1] : DEFAULT_INPUT_FILE;
        return read_file(input_dir() / day / file_name);
    };

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
};
