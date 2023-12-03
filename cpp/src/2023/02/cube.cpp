//
// Day 2: Cube Conundrum
// https://adventofcode.com/2023/day/1
//
// Created by Conor Pappas on 12/2/23.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <ranges>
#include <algorithm>

using namespace std;

const string INPUT_DIR = "/Users/conorpappas/current/advent_of_code/cpp/data/2023/02/";

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
auto split(const string& str, char delim) {
    return str
        | views::split(delim)
        | views::transform([](auto&& rng) { return string(rng.begin(), rng.end()); });
}

enum class CubeColor {
    RED,
    GREEN,
    BLUE
};

struct BallSet {
    int red_count = 0;
    int green_count = 0;
    int blue_count = 0;

    void add_count(CubeColor cubeColor, int count) {
        switch (cubeColor) {
            case CubeColor::RED:
                red_count += count;
                break;
            case CubeColor::BLUE:
                blue_count += count;
                break;
            case CubeColor::GREEN:
                green_count += count;
                break;
        }
    }
};

struct Game {
    int number;
    vector<BallSet> pulls;
};

CubeColor parseCubeColor(const string& cubeColorString) {
    if (cubeColorString == "red") return CubeColor::RED;
    if (cubeColorString == "blue") return CubeColor::BLUE;
    if (cubeColorString == "green") return CubeColor::GREEN;
    throw runtime_error("Invalid cube color");
}

const regex GAME_FORMAT = regex(R"(Game (\d+): (.*))");
const regex CUBE_PULL_FORMAT = regex(R"((\d+) (red|blue|green))");
constexpr char SET_DELIMITER = ';';
constexpr char CUBE_PULL_DELIMITER = ',';

constexpr BallSet DEFAULT_UNIVERSE = { 12, 13, 14};

BallSet parsePull(const string& pullString) {
    auto pull = BallSet();

    for (const string& ballString : split(pullString, CUBE_PULL_DELIMITER)) {
        auto match = smatch();
        regex_search(ballString, match, CUBE_PULL_FORMAT);
        if(match.empty()) throw runtime_error("Invalid cube pull format");

        const auto cube_count = stoi(match[1]);
        const auto cube_color = parseCubeColor(match[2]);
        pull.add_count(cube_color, cube_count);
   }
    return pull;
}

Game parseGame(const string& line) {
    auto match = smatch();
    regex_search(line, match, GAME_FORMAT);
    if(match.empty()) throw runtime_error("Invalid game format");

    const int game_number = stoi(match[1]);
    const string line_without_prefix = match[2];

    auto pull_itr = split(line_without_prefix, SET_DELIMITER) | views::transform(parsePull);

    return { game_number, vector(pull_itr.begin(), pull_itr.end())};
}

BallSet calculateMinSet(const Game& game) {
    auto min_set = BallSet();
    for (const auto& pull : game.pulls) {
        min_set.red_count = max(min_set.red_count, pull.red_count);
        min_set.blue_count = max(min_set.blue_count, pull.blue_count);
        min_set.green_count = max(min_set.green_count, pull.green_count);
    }
    return min_set;
}

int calculatePower(const BallSet& balls) {
    return balls.red_count * balls.blue_count * balls.green_count;
}

bool ValidatePull(const BallSet& pull, const BallSet& universe) {
    return pull.red_count <= universe.red_count
        && pull.blue_count <= universe.blue_count
        && pull.green_count <= universe.green_count;
}

bool ValidateGame(const Game& game, const BallSet& universe) {
    return ranges::all_of(game.pulls, [universe](const auto& pull) { return ValidatePull(pull, universe); });
}

void printPull(const BallSet& balls) {
    cout << "  Pull: " << balls.red_count << " red, " << balls.blue_count << " blue, " << balls.green_count << " green" << endl;
}

void printGame(const Game& game) {
    cout << "Game: " << game.number << endl;
    for (const auto& pull : game.pulls) {
        printPull(pull);
    }
}

int part_1(const vector<Game>& games) {
    auto validGames = games
        | views::filter([](const auto& game) { return ValidateGame(game, DEFAULT_UNIVERSE); });

    int gameNumberSum = 0;
    for(const auto& game : validGames) {
        gameNumberSum += game.number;
    }
    return gameNumberSum;
}

int part_2(const vector<Game>& games) {
    auto powers = games
        | views::transform(calculateMinSet)
        | views::transform(calculatePower);
    int powerSum = 0;
    for(const auto& power : powers) {
        powerSum += power;
    }
    return powerSum;
}

int main(int argc, char** argv) {
    const string filename = argc > 1 ? argv[1] : "input.txt";
    const vector<string> lines = readFile(INPUT_DIR + filename);

    const auto games = lines
        | views::transform(parseGame);

    cout << part_2(vector<Game>(games.begin(), games.end())) << endl;

}