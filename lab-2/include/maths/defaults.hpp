#pragma once

#include <cstddef>

namespace arms::maths::precision {

template <typename T>
struct Eps {
    static constexpr T VALUE = 1e-3;
};

template <>
struct Eps<float> {
    static constexpr float VALUE = static_cast<float>(1e-3);
};

template <>
struct Eps<double> {
    static constexpr double VALUE = static_cast<float>(1e-5);
};

template <>
struct Eps<long double> {
    static constexpr float VALUE = static_cast<float>(1e-7);
};

}  // namespace arms::maths::precision

namespace arms::maths::defaults {

template <typename T>
inline constexpr size_t DEFAULT_TERMS = 30;

template <>
inline constexpr size_t DEFAULT_TERMS<float> = 30;

template <>
inline constexpr size_t DEFAULT_TERMS<double> = 50;

template <>
inline constexpr size_t DEFAULT_TERMS<long double> = 70;

}  // namespace arms::maths::defaults
