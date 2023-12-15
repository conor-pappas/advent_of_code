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

        Node& add_node(const Label&, const Label& left, const Label& right);
        const Node& get_node(const Label&);
        const Node& traverse(const Node& node, const Instruction&);
        const Node& traverse(const Label&, const Instruction&);

        TraversalIterator traverse(const Node&, InstructionSet&);
        TraversalIterator traverse(const Label&, InstructionSet&);

        bool operator==(const Network&) const;

        friend std::ostream &operator<<(std::ostream&, const Network&);

    private:
        std::unordered_map<Label, Node> nodes {};
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
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = size_t;
        using value_type        = const Node;
        using pointer           = value_type*;
        using reference         = value_type&;

    public:
        TraversalIterator(const Node& current_node, Network& network, InstructionSet& instructions);

        bool operator==(const TraversalIterator& other) const;
        bool operator!=(const TraversalIterator& other) const;
        reference operator*() const;
        pointer operator->() const;
        TraversalIterator& operator++();
        TraversalIterator operator++(int) &;

    private:
        pointer current_node;
        Network& network;
        InstructionSet::iterator instruction;
    };
};
