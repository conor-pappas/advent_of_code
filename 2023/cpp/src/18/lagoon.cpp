// Day 17: Lavaduct Lagoon
// https://adventofcode.com/2023/day/18
//
// Created by Conor Pappas on 1/15/24.
//

#include <cassert>
#include <iostream>
#include <vector>

#include <input.hpp>
#include <point.hpp>

#include "parse/instruction.hpp"
#include "data_types/instruction.hpp"

using namespace std;
using namespace lagoon;

using coordinate = long;
using area = coordinate;
using InstructionCollection = vector<data_types::Instruction>;
using Point = support::Point<coordinate, 2>;
using PointCollection = vector<Point>;

PointCollection get_points(const InstructionCollection& instructions) {
    PointCollection points;
    Point point{0,0};
    for (const data_types::Instruction& instruction : instructions) {
        switch (instruction.direction) {
            case data_types::Direction::Right:
                point[0] += instruction.distance;
                break;
            case data_types::Direction::Down:
                point[1] += instruction.distance;
                break;
            case data_types::Direction::Left:
                point[0] -= instruction.distance;
                break;
            case data_types::Direction::Up:
                point[1] -= instruction.distance;
                break;
        }
        points.push_back(point);
    }
    return points;
}

area determinant(const Point& p, const Point& q) {
    return p[0]*q[1] - p[1]*q[0];
}

area total_area(const PointCollection& vertices) {
    area result = 0;
    for (size_t i = 0; i < vertices.size(); ++i) {
        result += determinant(vertices[i], vertices[(i+1)%vertices.size()]);
    }
    // Will need to flip the result if the points are in counter-clockwise order
    assert(result > 0);
    return result / 2;
}

size_t boundary_points(const InstructionCollection& instructions) {
    size_t result = 0;
    for (const auto& instruction : instructions) {
        result += instruction.distance;
    }
    return result;
}

size_t interior_points(const area A, const size_t b) {
    return A - b/2 + 1;
}

size_t total_points(const InstructionCollection& instructions) {
    const auto vertices = get_points(instructions);
    const auto boundary_count = boundary_points(instructions);
    const auto interior_count = interior_points(total_area(vertices), boundary_count);
    return interior_count + boundary_count;
}

size_t part_1(const InstructionCollection& instructions) {
    return total_points(instructions);
}

size_t part_2() {
    return 0;
}

int main(const int argc, const char** argv) {
    const vector<string> lines = support::read_input("18", argc, argv);

    const InstructionCollection instructions = parse::parse_instructions(lines);

    cout << "Part 1: " << part_1(instructions) << endl;
    cout << "Part 2: " << part_2() << endl;
}
