//
// Day 3: Gear Ratios
// https://adventofcode.com/2023/day/3
//
// Created by Conor Pappas on 12/3/23.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <algorithm>
#include <ranges>
#include <unordered_set>
#include <unordered_map>
#include <numeric>
#include <optional>

using namespace std;

const string INPUT_DIR = "/Users/conorpappas/current/advent_of_code/cpp/data/2023/03/";

// TODO: Move to lib
vector<string> readFile(const string& fileName) {
    ifstream file(fileName);

    if(!file) throw std::runtime_error("Could not open file");

    vector<string> lines;
    string line;
    while (getline(file, line)) {
        lines.push_back(line);
    }
    return lines;
}

// TODO: Move to lib
bool isDigit(char c) {
    return '0' <= c && c <= '9';
}

struct Point {
    int x;
    int y;
    char character;

    bool operator==(const Point& otherPoint) const
    {
        if (this->x == otherPoint.x && this->y == otherPoint.y && this->character == otherPoint.character) return true;
        else return false;
    }

    struct Hasher {
        size_t operator()(const Point& p) const{
            return std::hash<int>()(p.x)
                ^ std::hash<int>()(p.y)
                ^ std::hash<char>()(p.character);
        }
    };
};

typedef unordered_set<Point, Point::Hasher> PointSet;
typedef vector<vector<Point>> Plane;
template <typename T>
using PointMap = unordered_map<Point, T, Point::Hasher>;

optional<const Point> readPoint(const Plane& plane, const int x, const int y) {
    if (y < 0 || y >= plane.size()) return {};
    if (x < 0 || x >= plane[y].size()) return {};
    return make_optional<const Point>(plane[y][x]);
}

PointSet point_adjacencies(const Plane& plane, const Point point) {
    PointSet result;
    auto add_point = [&](const int x, const int y) {
        if(auto const found_point = readPoint(plane, x, y)) {
            result.insert(found_point.value());
        }
    };
    add_point(point.x - 1, point.y - 1);
    add_point(point.x, point.y - 1);
    add_point(point.x + 1, point.y - 1);
    add_point(point.x - 1, point.y);
    add_point(point.x + 1, point.y);
    add_point(point.x - 1, point.y + 1);
    add_point(point.x, point.y + 1);
    add_point(point.x + 1, point.y + 1);

    return result;
}

PointSet point_adjacencies(const Plane& plane, const PointSet& points) {
    PointSet result;
    for (const auto& point : points) {
        result.merge(point_adjacencies(plane, point));
    }
    return result;
}

struct PartNumber {
    PointSet points;

    [[nodiscard]] Point startingPoint() const {
        auto comp = [](const Point a, const Point b) { return a.x < b.x; };
        return *ranges::min_element(points, comp);
    }

    [[nodiscard]] Point endingPoint() const {
        auto comp = [](const Point a, const Point b) { return a.x < b.x; };
        return *ranges::max_element(points, comp);
    }

    [[nodiscard]] double value() const {
        double result = 0;
        const int ending_x = endingPoint().x;
        for (const auto& point : points) {
            const auto exponent = pow(10, (ending_x - point.x));
            result += (point.character - '0') * exponent;
        }
        return result;
    }
};

struct Symbol {
    const Point point;
};


class Grid {
public:
    const Plane plane;
    const vector<Symbol> symbols;
    const vector<PartNumber> partNumbers;
    PointMap<const PartNumber*> partNumberMap;
    PointMap<const Symbol*> symbolMap;

    Grid(Plane  plane, const vector<Symbol>& symbols, const vector<PartNumber>& partNumbers):
        plane(std::move(plane)),
        symbols(symbols),
        partNumbers(partNumbers)
    {
        buildPartNumberMap();
        buildSymbolMap();
    }

    [[nodiscard]] const PartNumber* partNumberAt(const Point& point) const {
        return partNumberMap.at(point);
    }

    [[nodiscard]] const Symbol* symbolAt(const Point& point) const {
        return symbolMap.at(point);
    }

    void buildPartNumberMap() {
        for (const auto& partNumber : partNumbers) {
            for (const auto& point : partNumber.points) {
                partNumberMap[point] = &partNumber;
            }
        }
    }

    void buildSymbolMap() {
        for (const auto& symbol : symbols) {
            symbolMap[symbol.point] = &symbol;
        }
    }
};

constexpr char EMPTY_CHARACTER = '.';

Plane parsePlane(const vector<string>& lines) {
    Plane result;
    for (int y = 0; y < lines.size(); y++) {
        const string& line = lines[y];
        vector<Point> row;
        for (int x = 0; x < line.length(); x++) {
            const char character = line[x];
            row.push_back({x, y, character});
        }
        result.push_back(row);
    }
    return result;
}

vector<PartNumber> parsePartNumbers(const Plane& plane) {
    vector<PartNumber> result;
    PartNumber currentPartNumber;
    for (const auto& row : plane) {
        for (const auto& point : row) {
            const auto& character = point.character;
            if(isDigit(character)) {
                currentPartNumber.points.insert(point);
            } else if(!currentPartNumber.points.empty()) {
                result.push_back(currentPartNumber);
                currentPartNumber = PartNumber();
            }
        }
    }
    return result;
}

vector<Symbol> parseSymbols(const Plane& plane) {
    vector<Symbol> result;
    for (const auto& row : plane) {
        for (const auto& point : row) {
            const auto character = point.character;
            if (character != EMPTY_CHARACTER && !isDigit(character)) {
                result.push_back({point});
            }
        }
    }
    return result;
}

Grid& parseGrid(const vector<string>& lines) {
    const Plane plane = parsePlane(lines);
    const vector<Symbol> symbols = parseSymbols(plane);
    const vector<PartNumber> partNumbers = parsePartNumbers(plane);
    return *new Grid{plane, symbols, partNumbers};
}

auto find_valid_part_numbers(const Grid& grid) {
    const auto isSymbol = [&grid](const Point& point) {
        return grid.symbolMap.contains(point);
    };
    const auto validPartNumber = [&grid, isSymbol](const PartNumber& partNumber) {
        auto adjacencies = point_adjacencies(grid.plane, partNumber.points);
        return ranges::any_of( adjacencies, isSymbol);
    };
    return grid.partNumbers | views::filter(validPartNumber);
}

}

void printGrid(const Grid& grid) {
    cout << "Grid:" << endl;
    for (const auto& row : grid.plane) {
        for (const auto& point : row) {
            cout << point.character;
        }
        cout << endl;
    }
    cout << "Part Numbers:" << endl;
    for (const auto& partNumber : grid.partNumbers) {
        cout << partNumber.value() << ", ";
    }
    cout << endl << "Symbols:" << endl;
    for (const auto& symbol : grid.symbols) {
        cout << symbol.point.character << ", ";
    }
    cout << endl;
}

double part_1(const Grid& grid) {
    double sum = 0;
    for (const auto& partNumber : find_valid_part_numbers(grid)) {
        sum += partNumber.value();
    }
    return sum;
}

int main(int argc, char** argv) {
    const string filename = argc > 1 ? argv[1] : "input.txt";
    const vector<string> lines = readFile(INPUT_DIR + filename);

    const Grid grid = parseGrid(lines);
    printGrid(grid);

    cout << part_1(grid) << endl;
}