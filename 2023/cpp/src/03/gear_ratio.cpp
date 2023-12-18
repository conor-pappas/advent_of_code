//
// Day 3: Gear Ratios
// https://adventofcode.com/2023/day/3
//
// Created by Conor Pappas on 12/3/23.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ranges>
#include <unordered_set>
#include <unordered_map>
#include <numeric>
#include <optional>

#include "input.hpp"

using namespace std;

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

class PartNumber {
public:
    const PointSet points;
    const Point startingPoint;
    const Point endingPoint;
    const int value;

    explicit PartNumber(PointSet  points):
        points(std::move(points)),
        startingPoint(calculateStartingPoint()),
        endingPoint(calculateEndingPoint()),
        value(calculateValue())
    {}

private:
    [[nodiscard]] Point calculateStartingPoint() const {
        auto comp = [](const Point a, const Point b) { return a.x < b.x; };
        return *ranges::min_element(points, comp);
    }

    [[nodiscard]] Point calculateEndingPoint() const {
        auto comp = [](const Point a, const Point b) { return a.x < b.x; };
        return *ranges::max_element(points, comp);
    }

    [[nodiscard]] int calculateValue() const {
        int result = 0;
        const int ending_x = endingPoint.x;
        for (const auto& point : points) {
            const int exponent = static_cast<int>(pow(10, (ending_x - point.x)));
            result += (point.character - '0') * exponent;
        }
        return result;
    }
};

struct Symbol {
    const Point point;
};

class Gear {
public:
    const Symbol symbol;
    const PartNumber partNumber_1;
    const PartNumber partNumber_2;

    [[nodiscard]] int ratio() const {
        return partNumber_1.value * partNumber_2.value;
    }
};

class Grid {
public:
    const Plane plane;
    const vector<Symbol> symbols;
    const vector<PartNumber> partNumbers;
    PointMap<const PartNumber*> partNumberLookup;
    PointMap<const Symbol*> symbolLookup;

    Grid(Plane plane, const vector<Symbol>& symbols, const vector<PartNumber>& partNumbers):
        plane(std::move(plane)),
        symbols(symbols),
        partNumbers(partNumbers)
    {
        buildPartNumberMap();
        buildSymbolMap();
    }

private:
    void buildPartNumberMap() {
        for (const auto& partNumber : partNumbers) {
            for (const auto& point : partNumber.points) {
                partNumberLookup[point] = &partNumber;
            }
        }
    }

    void buildSymbolMap() {
        for (const auto& symbol : symbols) {
            symbolLookup[symbol.point] = &symbol;
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
    PointSet currentPoints;
    for (const auto& row : plane) {
        for (const auto& point : row) {
            const auto& character = point.character;
            if(isDigit(character)) {
                currentPoints.insert(point);
            } else if(!currentPoints.empty()) {
                result.emplace_back(currentPoints);
                currentPoints = PointSet();
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
        return grid.symbolLookup.contains(point);
    };
    const auto validPartNumber = [&grid, isSymbol](const PartNumber& partNumber) {
        const auto adjacencies = point_adjacencies(grid.plane, partNumber.points);
        return ranges::any_of( adjacencies, isSymbol);
    };
    return grid.partNumbers | views::filter(validPartNumber);
}

class GearFinder {
public:
    Grid grid;

    explicit GearFinder(Grid grid):
        grid(std::move(grid))
    {}

    [[nodiscard]] vector<const Gear> findGears() const {
        vector<const Gear> result;
        for (const auto& symbol : grid.symbols) {
            if (const auto partNumbers = adjacentPartNumbers(symbol); partNumbers.size() == 2) {
                result.push_back({symbol, partNumbers[0], partNumbers[1]});
            }
        }
        return result;
    }

private:
    [[nodiscard]] PartNumber getPartNumber(const Point& point) const {
        return *grid.partNumberLookup.at(point);
    }

    [[nodiscard]] bool isPartNumber(const Point& point) const {
        return grid.partNumberLookup.contains(point);
    }

    [[nodiscard]] vector<const PartNumber> adjacentPartNumbers(const Symbol& symbol) const {
        PointSet locations;
        for (const auto& point :  point_adjacencies(grid.plane, symbol.point)) {
            if (isPartNumber(point)) {
                auto partNumber = getPartNumber(point);
                locations.insert(partNumber.startingPoint);
            }
        }
        vector<const PartNumber> partNumbers;
        for (const auto& location : locations) {
            partNumbers.push_back(getPartNumber(location));
        }
        return partNumbers;
    }
};

vector<const Gear> findGears(const Grid& grid) {
    return GearFinder(grid).findGears();
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
        cout << partNumber.value << ", ";
    }
    cout << endl << "Symbols:" << endl;
    for (const auto& symbol : grid.symbols) {
        cout << symbol.point.character << ", ";
    }
    cout << endl;
}

int part_1(const Grid& grid) {
    int sum = 0;
    for (const auto& partNumber : find_valid_part_numbers(grid)) {
        sum += partNumber.value;
    }
    return sum;
}

int part_2(const Grid& grid) {
    int sum = 0;
    for (const auto& gear : findGears(grid)) {
        sum += gear.ratio();
    }
    return sum;
}

int main(const int argc, const char** argv) {
    const vector<string> lines = support::read_input("03", argc, argv);

    const Grid grid = parseGrid(lines);
    printGrid(grid);

    cout << part_2(grid) << endl;
}
