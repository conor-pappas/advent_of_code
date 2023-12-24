//
// Created by Conor Pappas on 12/23/23.
//

#include "pattern.hpp"

#include <cassert>
#include <ostream>
#include <ranges>

#include "string.hpp"

namespace spring::data_types {
    Pattern::Pattern() = default;

    Pattern::run_size_t Pattern::peak() const { return m_numbers.front(); }
    void Pattern::pop() { m_numbers.pop_front(); }

    void Pattern::expand(const size_t n) {
        assert(n > 0);
        const auto begin = m_numbers.begin();
        const auto end = m_numbers.end();
        for (size_t i = 1; i < n; ++i) {
            m_numbers.insert(m_numbers.end(), begin, end);
        }
    }

    Pattern::iterator Pattern::begin() { return m_numbers.begin(); }
    Pattern::const_iterator Pattern::begin() const { return m_numbers.begin(); }
    Pattern::iterator Pattern::end() { return m_numbers.end(); }
    Pattern::const_iterator Pattern::end() const { return m_numbers.end(); }

    bool Pattern::empty() const { return m_numbers.empty(); }
    size_t Pattern::size() const { return m_numbers.size(); }

    size_t Pattern::Hasher::operator()(const Pattern& pattern) const {
        size_t hash = pattern.size();
        for (const auto& run_size : pattern) {
            hash ^= std::hash<run_size_t>{}(run_size);
        }
        return hash;
    }

    bool operator==(const Pattern& a, const Pattern& b) {
        return a.m_numbers == b.m_numbers;
    }

    std::ostream& operator<<(std::ostream& os, const Pattern& pattern) {
        auto to_string = std::views::transform([](const auto& run_size) {
            return std::to_string(run_size);
        });
        return os << '[' << support::join(pattern.m_numbers | to_string, ",") << ']';
    }
}
