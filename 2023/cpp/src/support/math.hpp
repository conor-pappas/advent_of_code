//
// Created by Conor Pappas on 12/18/23.
//

#pragma once

namespace support {
    template <typename T>
    T factorial(T);

    template<typename T>
    T factorial(T n) {
        static_assert(std::is_integral_v<T>, "T must be an integral type");
        T result = 1;
        if (n < 0) throw std::invalid_argument("n must be non-negative");
        if (n == 0) return result;
        for (int i = 2; i<=n; ++i) {
            result *= i;
        }
        return result;
    }
}
