//
// Created by Conor Pappas on 12/23/23.
//

#include "record.hpp"

#include <cassert>
#include <__algorithm/ranges_none_of.h>
#include <ostream>

namespace spring::data_types {
    Record::Record() = default;

    Record::iterator Record::begin() { return m_conditions.begin(); }
    Record::const_iterator Record::begin() const { return m_conditions.begin(); }
    Record::iterator Record::end() { return m_conditions.end(); }
    Record::const_iterator Record::end() const { return m_conditions.end(); }

    size_t Record::size() const { return m_conditions.size(); }

    Record& Record::operator+=(const Record& other) {
        m_conditions.insert(m_conditions.end(), other.begin(), other.end());
        return *this;
    }

    bool Record::is_concrete() const {
        auto pred = [](const Condition c) { return c ==  Condition::UNKNOWN; };
        return std::ranges::none_of(*this, pred);
    }

    void Record::expand(size_t n, const Record& delimiter) {
        assert(n > 0);
        const Record self(this->begin(), this->end());
        for (size_t i = 1; i < n; ++i) {
            *this += delimiter;
            *this += self;
        }
    }

    Record Record::join(const std::initializer_list<Record> records) {
        Record result;
        for (const auto& record : records) {
            result += record;
        }
        return result;
    }

    Condition Record::char_to_condition(char c) {
        return static_cast<Condition>(c);
    }

    size_t Record::Hasher::operator()(const Record& record) const{
        size_t hash = record.size();
        for (const auto& condition : record) {
            hash ^= std::hash<Condition>{}(condition);
        }
        return hash;
    }

    Record operator+(const Record& a, const Record& b) {
        Record result(a);
        return result += b;
    }

    bool operator==(const Record& a, const Record& b) {
        return a.m_conditions == b.m_conditions;
    }

    std::ostream& operator<<(std::ostream& os, const Record& record) {
        for (const auto& condition : record) {
            os << static_cast<char>(condition);
        }
        return os;
    }
};
