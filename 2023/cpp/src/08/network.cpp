//
// Created by Conor Pappas on 12/15/23.
//

#include "network.hpp"

#include <utility>

const wasteland::Network::NodeMap& wasteland::Network::get_nodes() const {
    return nodes;
}

wasteland::Network::Node& wasteland::Network::add_node(const Label& label, const Label& left, const Label& right) {
    return nodes[label] = {label, left, right};
}

const wasteland::Network::Node& wasteland::Network::get_node(const Label& label) const {
    return nodes.find(label)->second;
}

const wasteland::Network::Node& wasteland::Network::traverse(const Node& node, const Instruction& instruction) const {
    switch(instruction.get_direction()) {
        case Instruction::Direction::LEFT: return get_node(node.left);
        case Instruction::Direction::RIGHT: return get_node(node.right);
        default: throw std::invalid_argument("Invalid instruction");
    }
}

const wasteland::Network::Node& wasteland::Network::traverse(const Label& label, const Instruction& instruction) const {
    return traverse(get_node(label), instruction);
}

wasteland::Network::TraversalIterator wasteland::Network::traverse(const Node& start_node, const InstructionSet& instructions) {
    return { start_node, *this, instructions };
}

wasteland::Network::TraversalIterator wasteland::Network::traverse(const Label& start_label, const InstructionSet& instructions) {
    return traverse(get_node(start_label), instructions);
}

bool wasteland::Network::operator==(const Network& other) const {
    return nodes == other.nodes;
}

bool wasteland::Network::Node::operator==(const Node& other) const {
    return label == other.label && left == other.left && right == other.right;
}

wasteland::Network::Node::operator std::string() const {
    return label;
}

wasteland::Network::TraversalIterator::TraversalIterator() = default;

// TODO: network should be by value. But then on destruction we destroy the network.
wasteland::Network::TraversalIterator::TraversalIterator(const Node& current_node, Network& network, const InstructionSet& instructions):
    current_node(&current_node),
    network(&network),
    instruction(instructions.begin()) {}

wasteland::Instruction wasteland::Network::TraversalIterator::get_instruction() const {
    return *instruction;
}

bool wasteland::Network::TraversalIterator::operator==(const TraversalIterator& other) const {
    return network == other.network && *current_node == *other.current_node && instruction.mod_equals(other.instruction);
}

bool wasteland::Network::TraversalIterator::operator!=(const TraversalIterator& other) const {
    return !(*this == other);
}

wasteland::Network::TraversalIterator::reference wasteland::Network::TraversalIterator::operator*() const {
    return *current_node;
}

wasteland::Network::TraversalIterator::pointer wasteland::Network::TraversalIterator::operator->() const {
    return current_node;
}

wasteland::Network::TraversalIterator& wasteland::Network::TraversalIterator::operator++() {
    const Instruction instruciton = *(instruction++);
    current_node = &network->traverse(*current_node, instruciton);
    return *this;
}

wasteland::Network::TraversalIterator wasteland::Network::TraversalIterator::operator++(int) & {
    const TraversalIterator result = *this;
    ++*this;
    return result;
}

std::ostream& wasteland::operator<<(std::ostream& os, const Network& network) {
    for(const auto& [label, node]: network.nodes) {
        os << node << std::endl;
    }
    return os;
}

std::ostream& wasteland::operator<<(std::ostream& os, const Network::Node& node) {
    os << node.label << " [" << node.left << "," << node.right << "]";
    return os;
}
