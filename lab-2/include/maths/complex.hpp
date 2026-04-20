#pragma once

#include <cmath>
#include <concepts>
#include <cstddef>
#include <limits>

#include "maths/defaults.hpp"

namespace arms::maths {

using defaults::DEFAULT_TERMS;
using precision::Eps;

template <typename T, T (*TanFn)(T, size_t), T (*Log5Fn)(T, size_t), T (*LnFn)(T, size_t), T (*Log3Fn)(T, size_t)>
requires std::floating_point<T>
constexpr T complex(T x, size_t terms = DEFAULT_TERMS<T>) {
    if (x <= static_cast<T>(0)) {
        return TanFn(x, terms);
    }

    const T log5_x = Log5Fn(x, terms);
    const T ln_x = LnFn(x, terms);
    const T log3_x = Log3Fn(x, terms);

    if (std::isnan(log5_x) || std::isnan(ln_x) || std::isnan(log3_x)) {
        return std::numeric_limits<T>::quiet_NaN();
    }

    if (std::abs(log5_x) < Eps<T>::VALUE || std::abs(ln_x) < Eps<T>::VALUE) {
        return std::numeric_limits<T>::quiet_NaN();
    }

    const T value = log5_x / ln_x - log3_x;
    return (value * value / log5_x) * log3_x;
}

template <typename T, float (*TanFn)(float, size_t), float (*Log5Fn)(float, size_t), float (*LnFn)(float, size_t),
          float (*Log3Fn)(float, size_t)>
requires std::integral<T>
constexpr float complex(T x, size_t terms = DEFAULT_TERMS<T>) {
    return complex<float, TanFn, Log5Fn, LnFn, Log3Fn>(static_cast<float>(x), terms);
}

}  // namespace arms::maths
