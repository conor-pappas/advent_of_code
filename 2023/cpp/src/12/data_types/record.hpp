//
// Created by Conor Pappas on 12/22/23.
//

#pragma once

#include <vector>

#include "range_concepts.hpp"

namespace spring::data_types {
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

        struct Hasher;

        Record();
        explicit Record(support::random_access_range_of<Condition> auto&&);
        template<support::input_iterator_of<Condition> Itr>
        Record(Itr, Itr);
        constexpr Record(std::initializer_list<Condition>);

        [[nodiscard]] iterator begin();
        [[nodiscard]] const_iterator begin() const;
        [[nodiscard]] iterator end();
        [[nodiscard]] const_iterator end() const;

        [[nodiscard]] size_t size() const;

        Record& operator+=(const Record&);

        [[nodiscard]] bool is_concrete() const;

        void expand(size_t n, const Record&delimiter);

        template<Condition target>
        [[nodiscard]] static bool is_a(Condition& c);
        static Record join(std::initializer_list<Record>);
        static Condition char_to_condition(char);

        friend Record operator+(const Record&, const Record&);
        friend bool operator==(const Record&, const Record&);

        friend std::ostream& operator<<(std::ostream&, const Record&);

    private:
        std::vector<Condition> m_conditions;
    };

    constexpr Record::Record(const std::initializer_list<Condition> conditions):
        m_conditions(conditions) {}

    struct Record::Hasher {
        size_t operator()(const Record& record) const;
    };

    Record::Record(support::random_access_range_of<Condition> auto&& range):
        m_conditions(std::forward<decltype(range)>(range)) {}

    template<support::input_iterator_of<Condition> Itr>
    Record::Record(Itr begin, Itr end):
        m_conditions(begin, end) {}

    template<Condition target>
    bool Record::is_a(Condition& c) {
        return c == target;
    }
}
