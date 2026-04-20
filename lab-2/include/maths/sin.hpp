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
constexpr T abs(T x) {
    return x < T{0} ? -x : x;
}

template <typename T>
requires std::floating_point<T>
constexpr T sin(T x, size_t terms = DEFAULT_TERMS<T>) {
    if (std::isinf(x) || std::isnan(x)) {
        return std::numeric_limits<T>::quiet_NaN();
    }

    constexpr T PI = std::numbers::pi_v<T>;
    constexpr T TWO_PI = PI * 2;
    auto pi_shift = static_cast<int32_t>(x / TWO_PI);
    x += -pi_shift * TWO_PI;

    if (x > PI) {
        x -= TWO_PI;
    } else if (x < -PI) {
        x += TWO_PI;
    }

    T result = x;
    T current_x = x;
    T fac = 1;
    int16_t sign = 1;

    for (size_t it = 1; it < terms; ++it) {
        current_x *= x * x;
        fac *= (2 * it) * (2 * it + 1);
        sign *= -1;
        if (std::isnan(current_x) || std::isnan(fac) || std::isnan(result + sign * current_x / fac))
            break;
        result += sign * current_x / fac;
    }

    return result;
}

template <typename T>
requires std::integral<T>
constexpr float sin(T x, size_t terms = DEFAULT_TERMS<T>) {
    return sin(static_cast<float>(x), terms);
}

}  // namespace arms::maths
