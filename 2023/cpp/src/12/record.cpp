//
// Created by Conor Pappas on 12/23/23.
//

#include "record.hpp"

namespace spring {
    Record::Record() = default;

    Record::iterator Record::begin() { return m_conditions.begin(); }
    Record::const_iterator Record::begin() const { return m_conditions.begin(); }
    Record::iterator Record::end() { return m_conditions.end(); }
    Record::const_iterator Record::end() const { return m_conditions.end(); }

    Record& Record::operator+=(const Record& other) {
        m_conditions.insert(m_conditions.end(), other.begin(), other.end());
        return *this;
    }

    Record Record::join(const std::initializer_list<Record> records) {
        Record result;
        for (const auto& record : records) {
            result += record;
        }
        return result;
    }

    Record operator+(const Record& a, const Record& b) {
        Record result(a);
        return result += b;
    }
};
