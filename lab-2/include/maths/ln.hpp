#pragma once

#include <cmath>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <numbers>

#include "maths/defaults.hpp"

namespace arms::maths {

using defaults::DEFAULT_TERMS;

template <typename T>
requires std::floating_point<T>
constexpr T ln(T x, size_t terms = DEFAULT_TERMS<T>) {
    if (std::isnan(x) || x < T{0}) {
        return std::numeric_limits<T>::quiet_NaN();
    }

    if (x == T{0}) {
        return -std::numeric_limits<T>::infinity();
    }

    if (std::isinf(x)) {
        return std::numeric_limits<T>::infinity();
    }

    if (x == T{1}) {
        return T{0};
    }

    int32_t exp2 = 0;
    while (x >= T{2}) {
        x *= T{0.5};
        ++exp2;
    }
    while (x < T{1}) {
        x *= T{2};
        --exp2;
    }

    const T y = (x - T{1}) / (x + T{1});
    const T y2 = y * y;

    T current_y = y;
    T result = current_y;

    for (size_t it = 1; it < terms; ++it) {
        current_y *= y2;
        const T term = current_y / static_cast<T>(2 * it + 1);
        if (std::isnan(term) || std::isnan(result + term))
            break;
        result += term;
    }

    return static_cast<T>(2) * result + static_cast<T>(exp2) * std::numbers::ln2_v<T>;
}

template <typename T>
requires std::integral<T>
constexpr float ln(T x, size_t terms = DEFAULT_TERMS<T>) {
    return ln(static_cast<float>(x), terms);
}

}  // namespace arms::maths
