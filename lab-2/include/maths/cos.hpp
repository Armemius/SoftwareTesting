#pragma once

#include <cmath>
#include <concepts>
#include <cstddef>
#include <limits>
#include <numbers>

#include "maths/defaults.hpp"

namespace arms::maths {

using defaults::DEFAULT_TERMS;

template <typename T, T (*SinFn)(T, size_t)>
requires std::floating_point<T>
constexpr T cos(T x, size_t terms = DEFAULT_TERMS<T>) {
    if (std::isinf(x) || std::isnan(x)) {
        return std::numeric_limits<T>::quiet_NaN();
    }

    constexpr T HALF_PI = std::numbers::pi_v<T> / static_cast<T>(2);
    return SinFn(x + HALF_PI, terms);
}

template <typename T, float (*SinFn)(float, size_t)>
requires std::integral<T>
constexpr float cos(T x, size_t terms = DEFAULT_TERMS<T>) {
    return cos<float, SinFn>(static_cast<float>(x), terms);
}

}  // namespace arms::maths
