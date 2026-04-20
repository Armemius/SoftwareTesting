#include <gtest/gtest.h>
#include <cmath>
#include <cstddef>
#include <limits>
#include <numbers>
#include <type_traits>

#include <maths/cos.hpp>
#include <maths/defaults.hpp>
#include <maths/sin.hpp>

using arms::maths::precision::Eps;

template <typename T>
class CosTest : public testing::Test {};

using FloatingPointTypes = testing::Types<float, double, long double>;
TYPED_TEST_SUITE(CosTest, FloatingPointTypes);

template <typename T>
inline constexpr auto SIN_PTR = static_cast<T (*)(T, size_t)>(arms::maths::sin);

constexpr auto SIN_FLOAT_PTR = static_cast<float (*)(float, size_t)>(arms::maths::sin);

TYPED_TEST(CosTest, TestInfinityAndNan) {
    using T = TypeParam;
    constexpr auto INF = std::numeric_limits<T>::infinity();
    constexpr auto NaN = std::numeric_limits<T>::quiet_NaN();

    EXPECT_TRUE((std::isnan(arms::maths::cos<T, SIN_PTR<T>>(INF))));
    EXPECT_TRUE((std::isnan(arms::maths::cos<T, SIN_PTR<T>>(-INF))));
    EXPECT_TRUE((std::isnan(arms::maths::cos<T, SIN_PTR<T>>(NaN))));
}

TYPED_TEST(CosTest, TestCharacteristicPoints) {
    using T = TypeParam;
    constexpr auto PI = std::numbers::pi_v<T>;

    EXPECT_NEAR((arms::maths::cos<T, SIN_PTR<T>>(static_cast<T>(0.0))), static_cast<T>(1.0), Eps<T>::VALUE);
    EXPECT_NEAR((arms::maths::cos<T, SIN_PTR<T>>(PI / 2)), static_cast<T>(0.0), Eps<T>::VALUE);
    EXPECT_NEAR((arms::maths::cos<T, SIN_PTR<T>>(PI)), static_cast<T>(-1.0), Eps<T>::VALUE);
    EXPECT_NEAR((arms::maths::cos<T, SIN_PTR<T>>(2 * PI)), static_cast<T>(1.0), Eps<T>::VALUE);
}

TYPED_TEST(CosTest, TestRangeAgainstStdCos) {
    using T = TypeParam;
    constexpr T VALUES[] = {
        static_cast<T>(-4.6), static_cast<T>(-1.3), static_cast<T>(-0.228),
        static_cast<T>(0.228), static_cast<T>(0.322), static_cast<T>(1.337),
        static_cast<T>(1.7), static_cast<T>(2.28), static_cast<T>(3.22),
        static_cast<T>(4.2), static_cast<T>(5.13)
    };

    for (const T value : VALUES) {
        const auto expected = static_cast<T>(std::cos(static_cast<long double>(value)));
        EXPECT_NEAR((arms::maths::cos<T, SIN_PTR<T>>(value)), expected, Eps<T>::VALUE);
    }
}
