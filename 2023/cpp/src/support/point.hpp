//
// Created by Conor Pappas on 12/21/23.
//

#pragma once

#include <array>

#include "point_fwd.hpp"

namespace support {
    // TODO: points kinda need two types. One for their own scalars, and another for their difference scalars.
    // For example, you know all your points should be unsigned. But it's still useful to take a
    // difference between two points, which would be signed.
    // You need double the size of the original point for the difference, though.
    // But we can of course make assertions about the sizes of the concrete values.
    template<typename Scalar, std::size_t N>
    class Point {
    public:
        struct Hasher;

        constexpr Point();
        constexpr explicit Point(const std::array<Scalar, N>&);
        constexpr explicit Point(const Scalar&);
        constexpr Point(const Scalar&, const Scalar&);
        constexpr Point(const Scalar&, const Scalar&, const Scalar&);

        Scalar& x();
        Scalar& y();
        Scalar& z();
        const Scalar& x() const;
        const Scalar& y() const;
        const Scalar& z() const;

        static constexpr size_t size();

        Point operator-() const;
        Scalar& operator[](std::size_t);
        const Scalar& operator[](std::size_t) const;

        // TODO: Casting between points of different types

        Point& operator+=(const Point&);
        Point& operator-=(const Point&);
        Point& operator*=(const Scalar&);
        Point& operator/=(const Scalar&);

        // TODO: Add Point::distance methods (which take in a metric, defaulting to Euclidean);

        friend bool operator==<Scalar, N>(const Point&, const Point&);
        friend Point operator+<Scalar, N>(const Point&, const Point&);
        friend Point operator-<Scalar, N>(const Point&, const Point&);
        friend Point operator*<Scalar, N>(const Point&, const Scalar&);
        friend Point operator*<Scalar, N>(const Scalar&, const Point&);
        friend Point operator/<Scalar, N>(const Point&, const Scalar&);
    private:
        std::array<Scalar, N> m_coordinates {};
    };

    template<typename Scalar, std::size_t N>
    struct Point<Scalar, N>::Hasher {
        std::size_t operator()(const Point& point) const;
    };

    template<typename Scalar, std::size_t N>
    constexpr size_t Point<Scalar, N>::size() {
        return N;
    }

    template<typename Scalar, std::size_t N>
    constexpr Point<Scalar, N>::Point(): m_coordinates() {}

    template<typename Scalar, std::size_t N>
    constexpr Point<Scalar, N>::Point(const std::array<Scalar, N>& coords):
        m_coordinates(coords) {}

    template<typename Scalar, std::size_t N>
    constexpr Point<Scalar, N>::Point(const Scalar& x) {
        static_assert(N == 1, "Point<Scalar,N>::Point(const Scalar&) requires N == 1");
        m_coordinates = { x };
    }

    template<typename Scalar, std::size_t N>
    constexpr Point<Scalar, N>::Point(const Scalar& x, const Scalar& y) {
        static_assert(N == 2, "Point<Scalar,N>::Point(const Scalar&, const Scalar&) requires N == 2");
        m_coordinates = { x, y };
    }

    template<typename Scalar, std::size_t N>
    constexpr Point<Scalar, N>::Point(const Scalar& x, const Scalar& y, const Scalar& z) {
        static_assert(N == 3, "Point<Scalar,N>::Point(const Scalar&, const Scalar&) requires N == 3");
        m_coordinates = { x, y, z};
    }

    template<typename Scalar, std::size_t N>
    Scalar& Point<Scalar, N>::operator[](std::size_t i) {
        return m_coordinates[i];
    }

    template<typename Scalar, std::size_t N>
    const Scalar& Point<Scalar, N>::operator[](std::size_t i) const {
        return m_coordinates[i];
    }

    template<typename Scalar, std::size_t N>
    Scalar& Point<Scalar, N>::x() {
        static_assert(N >= 1, "Point<Scalar,N>::x() requires N >= 1");
        return m_coordinates[0];
    }

    template<typename Scalar, std::size_t N>
    Scalar& Point<Scalar, N>::y() {
        static_assert(N >= 2, "Point<Scalar,N>::y() requires N >= 2");
        return m_coordinates[1];
    }

    template<typename Scalar, std::size_t N>
    Scalar& Point<Scalar, N>::z() {
        static_assert(N >= 3, "Point<Scalar,N>::z() requires N >= 3");
        return m_coordinates[2];
    }

    template<typename Scalar, std::size_t N>
    const Scalar& Point<Scalar, N>::x() const {
        static_assert(N >= 1, "Point<Scalar,N>::x() requires N >= 1");
        return m_coordinates[0];
    }

    template<typename Scalar, std::size_t N>
    const Scalar& Point<Scalar, N>::y() const {
        static_assert(N >= 2, "Point<Scalar,N>::y() requires N >= 2");
        return m_coordinates[1];
    }

    template<typename Scalar, std::size_t N>
    const Scalar& Point<Scalar, N>::z() const {
        static_assert(N >= 3, "Point<Scalar,N>::z() requires N >= 3");
        return m_coordinates[2];
    }

    template<typename Scalar, std::size_t N>
    Point<Scalar, N> Point<Scalar, N>::operator-() const {
        Point result;
        for (std::size_t i = 0; i < N; ++i) {
            result[i] = -(*this)[i];
        }
        return result;
    }

    template<typename Scalar, std::size_t N>
    Point<Scalar, N>& Point<Scalar, N>::operator+=(const Point& other) {
        for (std::size_t i = 0; i < N; ++i) {
            (*this)[i] += other[i];
        }
        return *this;
    }

    template<typename Scalar, std::size_t N>
    Point<Scalar, N>& Point<Scalar, N>::operator-=(const Point& other) {
        for (std::size_t i = 0; i < N; ++i) {
            (*this)[i] -= other[i];
        }
        return *this;
    }

    template<typename Scalar, std::size_t N>
    Point<Scalar, N>& Point<Scalar, N>::operator*=(const Scalar& scalar) {
        for (std::size_t i = 0; i < N; ++i) {
            (*this)[i] *= scalar;
        }
        return *this;
    }

    template<typename Scalar, std::size_t N>
    Point<Scalar, N>& Point<Scalar, N>::operator/=(const Scalar& scalar) {
        assert(scalar != 0);
        for (std::size_t i = 0; i < N; ++i) {
            (*this)[i] /= scalar;
        }
        return *this;
    }

    template<typename Scalar, std::size_t N>
    std::size_t Point<Scalar, N>::Hasher::operator()(const Point& point) const {
        std::size_t result = 0;
        for (std::size_t i = 0; i < N; ++i) {
            result ^= std::hash<Scalar>()(point[i]);
        }
        return result;
    }

    template<typename Scalar, std::size_t N>
    bool operator==(const Point<Scalar, N>& a, const Point<Scalar, N>& b) {
        return a.m_coordinates == b.m_coordinates;
    }

    template<typename Scalar, std::size_t N>
    constexpr Point<Scalar, N> operator+(const Point<Scalar, N>& a, const Point<Scalar, N>& b) {
        Point<Scalar, N> result(a);
        return result += b;
    }

    template<typename Scalar, std::size_t N>
    constexpr Point<Scalar, N> operator-(const Point<Scalar, N>& point, const Point<Scalar, N>& scalar) {
        Point<Scalar, N> result(point);
        return result -= scalar;
    }

    template<typename Scalar, std::size_t N>
    constexpr Point<Scalar, N> operator*(const Point<Scalar, N>& point, const Scalar& scalar) {
        Point<Scalar, N> result(point);
        return result *= scalar;
    }

    template<typename Scalar, std::size_t N>
    constexpr Point<Scalar, N> operator*(const Scalar& scalar, const Point<Scalar, N>& point) {
        return point*scalar;
    }

    template<typename Scalar, std::size_t N>
    constexpr Point<Scalar, N> operator/(const Point<Scalar, N>& a, const Scalar& b) {
        Point<Scalar, N> result(a);
        return result /= b;
    }
}
