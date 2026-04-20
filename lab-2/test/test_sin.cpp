#include <gtest/gtest.h>
#include <cmath>
#include <limits>
#include <numbers>
#include <type_traits>

#include <maths/defaults.hpp>
#include <maths/sin.hpp>

using arms::maths::sin;
using arms::maths::precision::Eps;

template <typename T>
class SinTest : public testing::Test {};

using FloatingPointTypes = testing::Types<float, double, long double>;
TYPED_TEST_SUITE(SinTest, FloatingPointTypes);

template <typename T>
struct XY {
    T x;
    T y;
};

TYPED_TEST(SinTest, EquivalenceClassFiniteValuesReferenceTable) {
    using T = TypeParam;
    constexpr XY<T> CASES[] = {
        {static_cast<T>(-4.8), static_cast<T>(0.9961646088358407)},
        {static_cast<T>(-2.1), static_cast<T>(-0.8632093666488737)},
        {static_cast<T>(-0.8), static_cast<T>(-0.7173560908995228)},
        {static_cast<T>(-0.1), static_cast<T>(-0.09983341664682815)},
        {static_cast<T>(0.1), static_cast<T>(0.09983341664682815)},
        {static_cast<T>(0.8), static_cast<T>(0.7173560908995228)},
        {static_cast<T>(2.1), static_cast<T>(0.8632093666488737)},
        {static_cast<T>(4.8), static_cast<T>(-0.9961646088358407)},
    };

    for (const auto &c : CASES) {
        EXPECT_NEAR(sin(c.x), c.y, Eps<T>::VALUE);
    }
}

TYPED_TEST(SinTest, EquivalenceClassPeriodicity) {
    using T = TypeParam;
    constexpr T X = static_cast<T>(0.2357);
    constexpr auto PI = std::numbers::pi_v<T>;
    constexpr auto PERIOD = static_cast<T>(2) * PI;

    EXPECT_NEAR(sin(X), sin(X + PERIOD * 10), Eps<T>::VALUE);
    EXPECT_NEAR(sin(X), sin(X - PERIOD * 10), Eps<T>::VALUE);
}

TYPED_TEST(SinTest, EquivalenceClassOddSymmetry) {
    using T = TypeParam;
    constexpr T X = static_cast<T>(0.7345);

    EXPECT_NEAR(sin(-X), -sin(X), Eps<T>::VALUE);
}

TYPED_TEST(SinTest, BoundaryCharacteristicPoints) {
    using T = TypeParam;
    constexpr auto PI = std::numbers::pi_v<T>;

    EXPECT_NEAR(sin(static_cast<T>(0.0)), static_cast<T>(0.0), Eps<T>::VALUE);
    EXPECT_NEAR(sin(PI / 2), static_cast<T>(1.0), Eps<T>::VALUE);
    EXPECT_NEAR(sin(-PI / 2), static_cast<T>(-1.0), Eps<T>::VALUE);
    EXPECT_NEAR(sin(PI), static_cast<T>(0.0), Eps<T>::VALUE);
    EXPECT_NEAR(sin(static_cast<T>(2) * PI), static_cast<T>(0.0), Eps<T>::VALUE);
}

TYPED_TEST(SinTest, BoundaryNanAndInfinity) {
    using T = TypeParam;
    constexpr auto INF = std::numeric_limits<T>::infinity();
    constexpr auto NaN = std::numeric_limits<T>::quiet_NaN();

    EXPECT_TRUE(std::isnan(sin(INF)));
    EXPECT_TRUE(std::isnan(sin(-INF)));
    EXPECT_TRUE(std::isnan(sin(NaN)));
}

TEST(SinTestIntegral, EquivalenceClassIntegralOverload) {
    EXPECT_NEAR(sin(0), 0.0F, Eps<float>::VALUE);
    EXPECT_NEAR(sin(1), 0.8414709848078965F, Eps<float>::VALUE);
    EXPECT_NEAR(sin(-1), -0.8414709848078965F, Eps<float>::VALUE);
}
