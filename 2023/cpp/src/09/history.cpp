//
// Created by Conor Pappas on 12/18/23.
//

#include <ostream>

#include "history.hpp"

oasis::History::History() = default;

oasis::History::History(const std::vector<value_type>& values):
    m_values(values) { }

void oasis::History::add(const value_type& value) {
    m_values.push_back(value);
}

const std::vector<oasis::History::value_type>& oasis::History::get_values() const {
    return m_values;
}

std::size_t oasis::History::size() const {
    return m_values.size();
}

std::ostream& oasis::operator<<(std::ostream& os, const History& history) {
    for (const auto& value : history.m_values) {
        os << value << " ";
    }
    return os;
}
