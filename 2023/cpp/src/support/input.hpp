//
// Created by Conor Pappas on 12/17/23.
//

#pragma once

#include <vector>
#include <filesystem>
#include <fstream>

namespace support {
    inline const char* INPUT_DIR_ENV_VAR = "AOC_INPUT_DIR";
    const std::string INPUT_DIR = "/Users/conorpappas/dev/projects/current/advent_of_code/2023/input";
    const std::string DEFAULT_INPUT_FILE = "input.txt";

    [[nodiscard]] std::filesystem::path input_dir();
    [[nodiscard]] std::vector<std::string> read_file(const std::filesystem::path& fileName);
    [[nodiscard]] std::vector<std::string> read_input(const std::string& day, int argc, const char** argv);

    [[nodiscard]] std::vector<std::string> split(const std::string& str, const char& delim);
};
