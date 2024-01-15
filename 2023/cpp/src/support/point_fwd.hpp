//
// Created by Conor Pappas on 12/21/23.
//

#pragma once

#include <ostream>

namespace support {
    template<typename Scalar, std::size_t N>
    class Point;

    template<typename Scalar, std::size_t N>
    bool operator==(const Point<Scalar, N>&, const Point<Scalar, N>&);

    template<typename Scalar, std::size_t N>
    constexpr Point<Scalar, N> operator+(const Point<Scalar, N>&, const Point<Scalar, N>&);

    template<typename Scalar, std::size_t N>
    constexpr Point<Scalar, N> operator-(const Point<Scalar, N>&, const Point<Scalar, N>&);

    template<typename Scalar, std::size_t N>
    constexpr Point<Scalar, N> operator*(const Point<Scalar, N>&, const Scalar&);

    template<typename Scalar, std::size_t N>
    constexpr Point<Scalar, N> operator*(const Scalar&, const Point<Scalar, N>&);

    template<typename Scalar, std::size_t N>
    constexpr Point<Scalar, N> operator/(const Point<Scalar, N>&, const Scalar&);

    template<typename Scalar, std::size_t N>
    std::ostream& operator<<(std::ostream & os, const Point<Scalar, N>&);
};
