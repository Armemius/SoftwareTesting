#pragma once

#include <cmath>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <limits>
#include <numbers>
#include <stdexcept>
#include <vector>

#include "maths/defaults.hpp"

namespace arms::maths {

using defaults::DEFAULT_TERMS;
using precision::Eps;

template <typename T>
constexpr T abs(T x) {
    return x < T{0} ? -x : x;
}

template <typename T>
requires std::floating_point<T>
constexpr T cos(T x, size_t terms = DEFAULT_TERMS<T>) {
    T result{1.0};
    T current_x{1.0};
    int16_t sign = 1;
    T fac = 1;

    for (size_t it = 1; it < terms; ++it) {
        current_x *= x * x;
        fac *= (2 * it - 1) * (2 * it);
        sign *= -1;
        if (std::isnan(current_x) || std::isnan(fac) || std::isnan(result + sign * current_x / fac))
            break;
        result += sign * current_x / fac;
    }

    return result;
}

template <typename T>
requires std::integral<T>
constexpr float cos(T x, size_t terms = DEFAULT_TERMS<T>) {
    return cos(static_cast<float>(x), terms);
}

template <typename T>
requires std::floating_point<T>
constexpr T sec(T x, size_t terms = DEFAULT_TERMS<T>) {
    if (std::isinf(x) || std::isnan(x)) {
        return std::numeric_limits<T>::quiet_NaN();
    }

    if (x < 0) {
        x = -x;
    }

    constexpr T PI = std::numbers::pi_v<T>;
    auto pi_shift = static_cast<int32_t>(x / (PI * 2));
    x += -pi_shift * 2 * PI;

    if (abs(x - PI / 2) < Eps<T>::VALUE || abs(x - 3 * PI / 2) < Eps<T>::VALUE) {
        return std::numeric_limits<T>::quiet_NaN();
    }
    return 1 / cos(x, terms);
}

template <typename T>
requires std::integral<T>
constexpr float sec(T x, size_t terms = DEFAULT_TERMS<T>) {
    return sec(static_cast<float>(x), terms);
}

}  // namespace arms::maths
