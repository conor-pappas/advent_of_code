//
// Created by Conor Pappas on 12/18/23.
//

#pragma once

#include <vector>

#include "mahler_polynomial.hpp"
#include "math.hpp"

namespace oasis {
    template<std::signed_integral Scalar, std::unsigned_integral Degree = unsigned char>
    class MahlerPolynomial {
    public:
        class Binomial;

        MahlerPolynomial();
        explicit MahlerPolynomial(const std::vector<Scalar>& cooeficients);

        Scalar& operator[](Degree);
        const Scalar& operator[](Degree) const;

        [[nodiscard]] Scalar calculate(Scalar x) const;

        // TODO: Figure out how to forward declare this type of friend function.
        friend std::ostream& operator<<(std::ostream& os, const MahlerPolynomial& polynomial) {
            os << polynomial.m_cooeficients[0] << Binomial(0);
            for (size_t i = 1; i < polynomial.m_cooeficients.size(); ++i) {
                os << " + " << polynomial.m_cooeficients[i] << Binomial(i);
            }
            return os;
        }
    private:
        std::vector<Scalar> m_cooeficients;
    };

    template<std::signed_integral Scalar, std::unsigned_integral Degree>
    class MahlerPolynomial<Scalar, Degree>::Binomial {
    public:
        Binomial() = delete;
        explicit Binomial(const Degree& degree);

        [[nodiscard]] Degree getDegree() const;
        [[nodiscard]] Scalar calculate(Scalar x) const;

        friend std::ostream& operator<<(std::ostream& os, const Binomial& binomial) {
            os << "C(x," << static_cast<unsigned int>(binomial.m_degree) << ")";
            return os;
        }
    private:
        Degree m_degree { 0 };
        Scalar m_degree_factorial { 1 };
    };

    template<std::signed_integral Scalar, std::unsigned_integral Degree>
    MahlerPolynomial<Scalar, Degree>::Binomial::Binomial(const Degree& degree):
        m_degree(degree),
        m_degree_factorial(support::factorial(static_cast<Scalar>(degree))) {
    }

    template<std::signed_integral Scalar, std::unsigned_integral Degree>
    Degree MahlerPolynomial<Scalar, Degree>::Binomial::getDegree() const {
        return m_degree;
    }

    template<std::signed_integral Scalar, std::unsigned_integral Degree>
    Scalar MahlerPolynomial<Scalar, Degree>::Binomial::calculate(Scalar x) const {
        Scalar numerator = 1;
        for(Degree i = 0; i < m_degree; ++i) {
            numerator *= x - i;
        }
        // TODO: I _think_ this should always be integral. But we need to check.
        return numerator / m_degree_factorial;
    }

    template<std::signed_integral Scalar, std::unsigned_integral Degree>
    MahlerPolynomial<Scalar, Degree>::MahlerPolynomial() = default;

    template<std::signed_integral Scalar, std::unsigned_integral Degree>
    MahlerPolynomial<Scalar, Degree>::MahlerPolynomial(const std::vector<Scalar>& cooeficients):
        m_cooeficients(cooeficients) { }

    template<std::signed_integral Scalar, std::unsigned_integral Degree>
    Scalar& MahlerPolynomial<Scalar, Degree>::operator[](Degree degree) {
        return m_cooeficients[degree];
    }

    template<std::signed_integral Scalar, std::unsigned_integral Degree>
    const Scalar& MahlerPolynomial<Scalar, Degree>::operator[](Degree degree) const {
        return m_cooeficients[degree];
    }

    template<std::signed_integral Scalar, std::unsigned_integral Degree>
    Scalar MahlerPolynomial<Scalar, Degree>::calculate(Scalar x) const {
        Scalar result = 0;
        for(Degree i = 0; i< m_cooeficients.size(); ++i) {
            auto binomial = Binomial(i);
            auto cooeficient = m_cooeficients[i];
            result += (cooeficient * binomial.calculate(x));
        }
        return result;
    }
} // oasis
