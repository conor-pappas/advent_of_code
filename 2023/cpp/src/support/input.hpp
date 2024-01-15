//
// Created by Conor Pappas on 12/17/23.
//

#pragma once

#include <filesystem>
#include <vector>

#include "grid.hpp"

namespace support {
    inline const char* INPUT_DIR_ENV_VAR = "AOC_INPUT_DIR";
    const std::string INPUT_DIR = "/Users/conorpappas/dev/projects/current/advent_of_code/2023/input";
    const std::string DEFAULT_INPUT_FILE = "input.txt";

    [[nodiscard]] std::filesystem::path input_dir();
    [[nodiscard]] std::vector<std::string> read_file(const std::filesystem::path& fileName);
    [[nodiscard]] std::vector<std::string> read_input(const std::string& day, int argc, const char** argv);

    template<typename T>
    Grid<T> parse_grid(const std::vector<std::string>& lines);

    template<typename T>
    Grid<T> parse_grid(const std::vector<std::string>& lines, std::function<T(char)> parse);

    template<typename T>
    Grid<T> parse_grid(const std::vector<std::string>& lines) {
        return parse_grid<T>(lines, [](char c) { return static_cast<T>(c); });
    }

    template<typename T>
    Grid<T> parse_grid(const std::vector<std::string>& lines, std::function<T(char)> parse) {
        Grid<T> grid;
        for (const std::string& line : lines) {
            typename Grid<T>::value_type row;
            for (const char& c : line) {
                row.push_back(parse(c));
            }
            grid.push_back(row);
        }
        return grid;
    }
};
