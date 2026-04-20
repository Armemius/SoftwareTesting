#include <gtest/gtest.h>
#include <cmath>
#include <limits>
#include <numbers>
#include <type_traits>

#include <maths/defaults.hpp>
#include <maths/sin.hpp>

using arms::maths::precision::Eps;
using arms::maths::sin;

template <typename T>
class SinTest : public testing::Test {};

using FloatingPointTypes = testing::Types<float, double, long double>;
TYPED_TEST_SUITE(SinTest, FloatingPointTypes);

TYPED_TEST(SinTest, TestInfinityAndNan) {
    using T = TypeParam;
    constexpr auto INF = std::numeric_limits<T>::infinity();
    constexpr auto NaN = std::numeric_limits<T>::quiet_NaN();

    EXPECT_TRUE(std::isnan(sin(INF)));
    EXPECT_TRUE(std::isnan(sin(-INF)));
    EXPECT_TRUE(std::isnan(sin(NaN)));
}

TYPED_TEST(SinTest, TestZero) {
    using T = TypeParam;

    EXPECT_NEAR(sin(static_cast<T>(0.0)), static_cast<T>(0.0), Eps<T>::VALUE);
}

TYPED_TEST(SinTest, TestCharacteristicPoints) {
    using T = TypeParam;
    constexpr auto PI = std::numbers::pi_v<T>;

    EXPECT_NEAR(sin(PI / 2), static_cast<T>(1.0), Eps<T>::VALUE);
    EXPECT_NEAR(sin(-PI / 2), static_cast<T>(-1.0), Eps<T>::VALUE);
    EXPECT_NEAR(sin(PI), static_cast<T>(0.0), Eps<T>::VALUE);
    EXPECT_NEAR(sin(2 * PI), static_cast<T>(0.0), Eps<T>::VALUE);
    EXPECT_NEAR(sin(3 * PI / 2), static_cast<T>(-1.0), Eps<T>::VALUE);
}

TYPED_TEST(SinTest, TestOddFunction) {
    using T = TypeParam;
    constexpr T X = static_cast<T>(0.7345);

    EXPECT_NEAR(sin(-X), -sin(X), Eps<T>::VALUE);
}

TYPED_TEST(SinTest, TestIntervals) {
    using T = TypeParam;
    constexpr T INITIAL = static_cast<T>(0.1459);
    constexpr auto PI = std::numbers::pi_v<T>;
    constexpr auto INTERVAL = 2 * PI;

    EXPECT_NEAR(sin(INITIAL), sin(INITIAL - INTERVAL * 30), Eps<T>::VALUE);
    EXPECT_NEAR(sin(INITIAL), sin(INITIAL - INTERVAL * 20), Eps<T>::VALUE);
    EXPECT_NEAR(sin(INITIAL), sin(INITIAL - INTERVAL * 10), Eps<T>::VALUE);
    EXPECT_NEAR(sin(INITIAL), sin(INITIAL + INTERVAL * 10), Eps<T>::VALUE);
    EXPECT_NEAR(sin(INITIAL), sin(INITIAL + INTERVAL * 20), Eps<T>::VALUE);
    EXPECT_NEAR(sin(INITIAL), sin(INITIAL + INTERVAL * 30), Eps<T>::VALUE);
}

TYPED_TEST(SinTest, TestRange) {
    using T = TypeParam;
    constexpr T VALUES[] = {
        static_cast<T>(-4.6), static_cast<T>(-1.3), static_cast<T>(-0.228),
        static_cast<T>(0.228), static_cast<T>(0.322), static_cast<T>(1.337),
        static_cast<T>(1.7), static_cast<T>(2.28), static_cast<T>(3.22),
        static_cast<T>(4.2), static_cast<T>(5.13)
    };

    for (const T value : VALUES) {
        const auto expected = static_cast<T>(std::sin(static_cast<long double>(value)));
        EXPECT_NEAR(sin(value), expected, Eps<T>::VALUE);
    }
}

TEST(SinTestIntegral, TestIntegralInput) {
    EXPECT_NEAR(sin(0), 0.0f, Eps<float>::VALUE);
    EXPECT_NEAR(sin(1), std::sin(1.0f), Eps<float>::VALUE);
    EXPECT_NEAR(sin(-1), std::sin(-1.0f), Eps<float>::VALUE);
}
