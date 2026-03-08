#pragma once

#include <concepts>
#include <cstdint>

namespace arms::maths {

long double cos(long double x, uint32_t terms = 30);

long double sec(long double x, uint32_t terms = 30);

}  // namespace arms::maths
