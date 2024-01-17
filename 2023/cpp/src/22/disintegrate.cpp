// Day 22: Sand Slabs
// https://adventofcode.com/2023/day/22
//
// Created by Conor Pappas on 1/16/24.
//

#include <cassert>
#include <iostream>
#include <vector>

#include <input.hpp>
#include <map>
#include <set>
#include <graph/adjacency_list.hpp>

#include "data_types/ground.hpp"
#include "parse.hpp"

using namespace std;
using namespace disintegrate;
using namespace data_types;

using SupportingGraph = support::graph::AdjacencyList<Block>;

Ground build_ground(const vector<Block>& blocks) {
    coordinate_t max_x {};
    coordinate_t max_y {};
    for(const auto block : blocks) {
        max_x = max(max_x, block.x.max);
        max_y = max(max_y, block.y.max);
    }
    Ground ground {};
    ground.resize(max_x + 1, max_y + 1);
    return ground;
}

SupportingGraph build_supporting_graph(vector<Block>& blocks) {
    SupportingGraph supporting_graph;
    for(const auto& block : blocks) add_vertex(supporting_graph, block);
    return supporting_graph;
}

using block_ptr = std::shared_ptr<Block>;
using BlockIndex = map<block_ptr, size_t>;

BlockIndex build_block_index(vector<Block>& blocks) {
    BlockIndex result;
    for(size_t i = 0; i < blocks.size(); ++i) {
        result[make_shared<Block>(blocks[i])] = i;
    }
    return result;
}

vector<block_ptr> build_falling_queue(const BlockIndex& block_ptrs) {
    std::vector<block_ptr> result;
    for(const auto& [block, index] : block_ptrs) {
        result.push_back(block);
    }
    const auto compare = [](const block_ptr& a, const block_ptr& b) {
        return a->z.min < b->z.min;
    };
    ranges::sort(result, compare);
    return result;
}

void fall_blocks(SupportingGraph& graph, Ground& ground, const BlockIndex& block_index) {
    const auto falling_queue = build_falling_queue(block_index);
    for(const auto& block : falling_queue) {
        auto supporting_blocks = fall_onto(block, ground);
        for(const auto& supporting_block : supporting_blocks) {
            if (!supporting_block) continue;
            add_arc(graph,block_index.at(block), block_index.at(supporting_block));
        }
    }
}

size_t part_1(Ground& ground, vector<Block>& blocks) {
    auto graph = build_supporting_graph(blocks);
    const auto block_index = build_block_index(blocks);
    fall_blocks(graph, ground, block_index);
    vector disintegratable(blocks.size(), true);
    for(const auto& [block,index] : block_index) {
        const auto degree = out_degree(graph, index);
        if (degree == 1) {
            const auto supporting_index = target(graph, *out_arcs(graph, index).begin());
            disintegratable[supporting_index] = false;
        } else if (degree == 0) {
            assert(block->z.min == 1);
        }
    }
    size_t result = 0;
    for(const auto& disintegratable_block : disintegratable) {
        if (disintegratable_block) ++result;
    }
    return result;
}

size_t part_2() {
    return 0;
}

int main(const int argc, const char** argv) {
    const vector<string> lines = support::read_input("22", argc, argv);

    auto blocks = parse::parse_block(lines);
    auto ground = build_ground(blocks);

    cout << "Part 1: " << part_1(ground, blocks) << endl;
    cout << "Part 2: " << part_2() << endl;
}
