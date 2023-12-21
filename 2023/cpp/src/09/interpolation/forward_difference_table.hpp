//
// Created by Conor Pappas on 12/18/23.
//

#pragma once

#include <__algorithm/ranges_any_of.h>

#include "mahler_polynomial.hpp"

namespace oasis {
    template<std::signed_integral Scalar>
    class ForwardDifferenceTable {
    public:
        using Row = std::vector<Scalar>;
        using Table = std::vector<Row>;

        explicit ForwardDifferenceTable(Row);

        [[nodiscard]] MahlerPolynomial<Scalar> getPolynomial() const;

    private:
        Table m_table;

        class Private;
    };

    template<std::signed_integral Scalar>
    class ForwardDifferenceTable<Scalar>::Private {
    public:
        static void fill_table(ForwardDifferenceTable&);
        static auto fill_next_row(ForwardDifferenceTable&);
    };

    template<std::signed_integral Scalar>
    ForwardDifferenceTable<Scalar>::ForwardDifferenceTable(Row input_row):
        m_table({input_row}) {
        Private::fill_table(*this);
    };

    template<std::signed_integral Scalar>
     MahlerPolynomial<Scalar> ForwardDifferenceTable<Scalar>::getPolynomial() const {
        std::vector<Scalar> result;
        result.reserve(m_table.size());
        for (const auto& row : m_table) {
            result.push_back(row[0]);
        }
        return MahlerPolynomial<Scalar>(result);
    }

    template<std::signed_integral Scalar>
    void ForwardDifferenceTable<Scalar>::Private::fill_table(ForwardDifferenceTable& polynomial) {
        auto last_row = polynomial.m_table.back();
        while(std::ranges::any_of(last_row, [](auto x) { return x != 0; })) {
            last_row = fill_next_row(polynomial);
        }
    }

    template<std::signed_integral Scalar>
    auto ForwardDifferenceTable<Scalar>::Private::fill_next_row(ForwardDifferenceTable& polynomial) {
        auto last_row = polynomial.m_table.back();
        auto& next_row = polynomial.m_table.emplace_back();
        if(last_row.size() <= 1) {
            last_row.push_back(0);
            return next_row;
        }
        for(size_t i = 0; i < last_row.size() - 1; i++) {
            next_row.push_back(last_row[i + 1] - last_row[i]);
        }
        return next_row;
    }
}; // oasis
