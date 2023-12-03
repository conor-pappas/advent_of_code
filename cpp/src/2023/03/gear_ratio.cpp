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
#include <unordered_set>

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
constexpr Point INVALID_POINT = Point{-1, -1, ' '};



typedef unordered_set<Point, Point::Hasher> PointSet;
typedef vector<vector<Point>> Plane;

Point readPoint(const Plane& plane, const int x, const int y) {
    if (y < 0 || y >= plane.size()) return INVALID_POINT;
    if (x < 0 || x >= plane[y].size()) return INVALID_POINT;
    return plane[y][x];
}

struct PartNumber {
    vector<Point> points;

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

    [[nodiscard]] PointSet adjacencies(const Plane& plane) const {
        PointSet result;
        for (const auto& point : points) {
            result.insert(readPoint(plane, point.x - 1, point.y - 1));
            result.insert(readPoint(plane, point.x, point.y - 1));
            result.insert(readPoint(plane, point.x + 1, point.y - 1));
            result.insert(readPoint(plane, point.x - 1, point.y));
            result.insert(readPoint(plane, point.x + 1, point.y));
            result.insert(readPoint(plane, point.x - 1, point.y + 1));
            result.insert(readPoint(plane, point.x, point.y + 1));
            result.insert(readPoint(plane, point.x + 1, point.y + 1));
        }
        return result;
    }
};

struct Symbol {
    const Point point;
};

struct Grid {
    Plane plane;
    vector<Symbol> symbols;
    vector<PartNumber> partNumbers;
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
                currentPartNumber.points.push_back(point);
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

vector<const PartNumber*> find_valid_part_numbers(const Grid& grid) {
    PointSet symbolPoints;
    for (const Symbol symbol : grid.symbols) {
        symbolPoints.insert(symbol.point);
    }

    vector<const PartNumber*> validPartNumbers;
    for (const auto& partNumber : grid.partNumbers) {
        for (const auto& adjacency : partNumber.adjacencies(grid.plane)) {
            if (symbolPoints.contains(adjacency)) {
                validPartNumbers.push_back(&partNumber);
                break;
            }
        }
    }
    return validPartNumbers;
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
    const auto validPartNumbers = find_valid_part_numbers(grid);
    double sum = 0;
    for (const auto& partNumber : validPartNumbers) {
        sum += partNumber->value();
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