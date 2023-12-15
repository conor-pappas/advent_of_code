//
// Created by Conor Pappas on 12/15/23.
//

#include "network.hpp"

wasteland::Network::Node& wasteland::Network::add_node(const Label& label, const Label& left, const Label& right) {
    return nodes[label] = {label, left, right};
}

const wasteland::Network::Node& wasteland::Network::get_node(const Label& label) {
    return nodes[label];
}

const wasteland::Network::Node& wasteland::Network::traverse(const Node& node, const Instruction& instruction) {
    switch(instruction.get_direction()) {
        case Instruction::Direction::LEFT: return get_node(node.left);
        case Instruction::Direction::RIGHT: return get_node(node.right);
        default: throw std::invalid_argument("Invalid instruction");
    }
}

const wasteland::Network::Node& wasteland::Network::traverse(const Label& label, const Instruction& instruction) {
    return traverse(get_node(label), instruction);
}

wasteland::Network::TraversalIterator wasteland::Network::traverse(const Node& start_node, InstructionSet& instructions) {
    return { start_node, *this, instructions };
}

wasteland::Network::TraversalIterator wasteland::Network::traverse(const Label& start_label, InstructionSet& instructions) {
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

wasteland::Network::TraversalIterator::TraversalIterator(const Node& current_node, Network& network, InstructionSet& instructions):
    current_node(&current_node),
    network(network),
    instruction(instructions.begin()) {}

bool wasteland::Network::TraversalIterator::operator==(const TraversalIterator& other) const {
    return network == other.network && instruction == other.instruction;
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
    current_node = &network.traverse(*current_node, instruciton);
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
