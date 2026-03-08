#include "maths/maths.hpp"

#include <cstddef>

namespace arms::maths {

long double cos(long double x, uint32_t terms) {
    long double res{1.0};
    long double term{1.0};
    int sign{1};

    for (uint32_t k = 1; k < terms; ++k) {
        term *= x * x / (2 * k * (2 * k - 1));
        sign = -sign;
        res += sign * term;
    }

    return res;
}

long double sec(long double x, uint32_t terms) {
    return 1.0 / cos(x, terms);
}

}  // namespace arms::maths
