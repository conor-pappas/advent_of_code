//
// Created by Conor Pappas on 12/22/23.
//

#pragma once

#include <vector>

#include "range_concepts.hpp"

namespace spring {
    // TODO: Move into Record
    enum class Condition: char {
        OPERATIONAL = '.',
        DAMAGED = '#',
        UNKNOWN = '?'
    };

    class Record {
    public:
        using iterator = std::vector<Condition>::iterator;
        using const_iterator = std::vector<Condition>::const_iterator;
        Record();
        explicit Record(support::random_access_range_of<Condition> auto&&);
        template<support::input_iterator_of<Condition> Itr>
        Record(Itr, Itr);

        [[nodiscard]] iterator begin();
        [[nodiscard]] const_iterator begin() const;
        [[nodiscard]] iterator end();
        [[nodiscard]] const_iterator end() const;

        Record& operator+=(const Record&);

        static Record join(std::initializer_list<Record>);

        friend Record operator+(const Record&, const Record&);

    private:
        std::vector<Condition> m_conditions;
    };

    Record::Record(support::random_access_range_of<Condition> auto&& range):
        m_conditions(std::forward<decltype(range)>(range)) {}

    template<support::input_iterator_of<Condition> Itr>
    Record::Record(Itr begin, Itr end):
        m_conditions(begin, end) {}

    constexpr Condition char_to_condition(char c) {
        return static_cast<Condition>(c);
    }
}
