#pragma once

#include <cmath>
#include <concepts>
#include <cstddef>
#include <limits>

#include "maths/defaults.hpp"

namespace arms::maths {

using defaults::DEFAULT_TERMS;

template <typename T, T (*LnFn)(T, size_t)>
requires std::floating_point<T>
constexpr T log_3(T x, size_t terms = DEFAULT_TERMS<T>) {
    const T denominator = LnFn(static_cast<T>(3), terms);
    if (std::isnan(denominator) || denominator == T{0}) {
        return std::numeric_limits<T>::quiet_NaN();
    }

    const T numerator = LnFn(x, terms);
    if (std::isnan(numerator)) {
        return std::numeric_limits<T>::quiet_NaN();
    }

    return numerator / denominator;
}

template <typename T, T (*LnFn)(T, size_t)>
requires std::floating_point<T>
constexpr T log_5(T x, size_t terms = DEFAULT_TERMS<T>) {
    const T denominator = LnFn(static_cast<T>(5), terms);
    if (std::isnan(denominator) || denominator == T{0}) {
        return std::numeric_limits<T>::quiet_NaN();
    }

    const T numerator = LnFn(x, terms);
    if (std::isnan(numerator)) {
        return std::numeric_limits<T>::quiet_NaN();
    }

    return numerator / denominator;
}

template <typename T, float (*LnFn)(float, size_t)>
requires std::integral<T>
constexpr float log_3(T x, size_t terms = DEFAULT_TERMS<T>) {
    return log_3<float, LnFn>(static_cast<float>(x), terms);
}

template <typename T, float (*LnFn)(float, size_t)>
requires std::integral<T>
constexpr float log_5(T x, size_t terms = DEFAULT_TERMS<T>) {
    return log_5<float, LnFn>(static_cast<float>(x), terms);
}

}  // namespace arms::maths
