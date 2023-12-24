//
// Created by Conor Pappas on 12/22/23.
//

#pragma once

#include <deque>

#include "range_concepts.hpp"

namespace spring::data_types {
    class Pattern {
    public:
        using run_size_t = size_t;
        using iterator = std::deque<run_size_t>::iterator;
        using const_iterator = std::deque<run_size_t>::const_iterator;

        struct Hasher;

        Pattern();
        explicit Pattern(support::input_range_of<run_size_t> auto&&);
        template<support::input_iterator_of<run_size_t> Itr>
        Pattern(Itr, Itr);

        [[nodiscard]] run_size_t peak() const;
        void pop();

        void expand(size_t n);

        [[nodiscard]] iterator begin();
        [[nodiscard]] const_iterator begin() const;
        [[nodiscard]] iterator end();
        [[nodiscard]] const_iterator end() const;

        [[nodiscard]] size_t size() const;
        [[nodiscard]] bool empty() const;

        friend bool operator==(const Pattern&, const Pattern&);

        friend std::ostream& operator<<(std::ostream&, const Pattern&);

    private:
        std::deque<run_size_t> m_numbers;
    };

    struct Pattern::Hasher {
        size_t operator()(const Pattern& pattern) const;
    };

    Pattern::Pattern(support::input_range_of<run_size_t> auto&& range):
        m_numbers(std::forward<decltype(range)>(range)) {}

    template<support::input_iterator_of<Pattern::run_size_t> Itr>
    Pattern::Pattern(Itr begin, Itr end):
        m_numbers(begin, end) {}
}
