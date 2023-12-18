//
// Created by Conor Pappas on 12/15/23.
//

#pragma once

#include <string>
#include <unordered_map>

#include "instruction_set.hpp"

namespace wasteland {
    class Network {
    public:
        class Node;
        class TraversalIterator;
        using Label = std::string;
        using NodeMap = std::unordered_map<Label, Node>;

        [[nodiscard]] const NodeMap& get_nodes() const;

        Node& add_node(const Label&, const Label& left, const Label& right);
        [[nodiscard]] const Node& get_node(const Label&) const;
        [[nodiscard]] const Node& traverse(const Node& node, const Instruction&) const;
        [[nodiscard]] const Node& traverse(const Label&, const Instruction&) const;

        // TODO: should be const, but we are passing a network ref to the iterator.
        [[nodiscard]] TraversalIterator traverse(const Node&, const InstructionSet&);
        [[nodiscard]] TraversalIterator traverse(const Label&, const InstructionSet&) ;

        [[nodiscard]] bool operator==(const Network&) const;

        friend std::ostream &operator<<(std::ostream&, const Network&);

    private:
        NodeMap nodes {};
    };

    class Network::Node {
    public:
        Label label;
        Label left;
        Label right;

        bool operator==(const Node&) const;
        explicit operator Label() const;

        friend std::ostream &operator<<(std::ostream&, const Node&);
    };

    // TODO: Make an input_iterator and fix const-correctness. Network::traverse() should return a const_iterator.
    class Network::TraversalIterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = ptrdiff_t;
        using value_type        = const Node;
        using pointer           = value_type*;
        using reference         = value_type&;

        TraversalIterator();
        TraversalIterator(const Node& current_node, Network& network, const InstructionSet& instructions);

        [[nodiscard]] Instruction get_instruction() const;

        [[nodiscard]] bool operator==(const TraversalIterator& other) const;
        [[nodiscard]] bool operator!=(const TraversalIterator& other) const;
        [[nodiscard]] reference operator*() const;
        [[nodiscard]] pointer operator->() const;
        TraversalIterator& operator++();
        TraversalIterator operator++(int) &;

        friend std::ostream& operator<<(std::ostream& os, const TraversalIterator& iterator) {
            const char instruction = static_cast<char>(*iterator.instruction);
            os << "TraversalIterator(" << *iterator.current_node << ", " << instruction << ")";
            return os;
        }

    private:
        pointer current_node {};
        Network* network {};
        InstructionSet::iterator instruction {};
    };
};
