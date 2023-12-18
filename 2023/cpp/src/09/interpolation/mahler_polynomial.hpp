//
// Created by Conor Pappas on 12/18/23.
//

#pragma once

namespace oasis {

class MahlerPolynomial {
public:
    using Scalar = long;

    class Binomial {
    public:
        Binomial();
        Binomial(unsigned int degree);

        unsigned int getDegree() const;

        Scalar calculate(Scalar x) const;

    private:
        unsigned int degree { 0 };
    };

    MahlerPolynomial();
    MahlerPolynomial(const vector<Scalar>& cooeficients);


private:
    vector<Scalar> cooeficients;

};

} // oasis
