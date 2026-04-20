#pragma once

#include <cmath>
#include <concepts>
#include <cstddef>
#include <limits>

#include "maths/cos.hpp"
#include "maths/defaults.hpp"

namespace arms::maths {

using defaults::DEFAULT_TERMS;
using precision::Eps;

template <typename T, T (*SinFn)(T, size_t), T (*CosFn)(T, size_t)>
requires std::floating_point<T>
constexpr T tan(T x, size_t terms = DEFAULT_TERMS<T>) {
    if (std::isinf(x) || std::isnan(x)) {
        return std::numeric_limits<T>::quiet_NaN();
    }

    const T sin_value = SinFn(x, terms);
    const T cos_value = CosFn(x, terms);

    if (std::isnan(sin_value) || std::isnan(cos_value) || std::abs(cos_value) < Eps<T>::VALUE) {
        return std::numeric_limits<T>::quiet_NaN();
    }

    return sin_value / cos_value;
}

template <typename T, float (*SinFn)(float, size_t), float (*CosFn)(float, size_t)>
requires std::integral<T>
constexpr float tan(T x, size_t terms = DEFAULT_TERMS<T>) {
    return tan<float, SinFn, CosFn>(static_cast<float>(x), terms);
}

}  // namespace arms::maths
