#include <gtest/gtest.h>
#include <cmath>
#include <limits>
#include <type_traits>

#include <maths/defaults.hpp>
#include <maths/ln.hpp>

using arms::maths::ln;
using arms::maths::precision::Eps;

template <typename T>
class LnTest : public testing::Test {};

using FloatingPointTypes = testing::Types<float, double, long double>;
TYPED_TEST_SUITE(LnTest, FloatingPointTypes);

template <typename T>
struct XY {
    T x;
    T y;
};

TYPED_TEST(LnTest, EquivalenceClassPositiveLessThanOne) {
    using T = TypeParam;
    constexpr XY<T> CASES[] = {
        {static_cast<T>(0.1), static_cast<T>(-2.3025850929940455)},
        {static_cast<T>(0.25), static_cast<T>(-1.3862943611198906)},
        {static_cast<T>(0.5), static_cast<T>(-0.6931471805599453)},
        {static_cast<T>(0.75), static_cast<T>(-0.2876820724517809)},
    };
    constexpr T TOL = static_cast<T>(Eps<T>::VALUE * 5);

    for (const auto& c : CASES) {
        const T actual = ln(c.x);
        EXPECT_LT(actual, static_cast<T>(0.0));
        EXPECT_NEAR(actual, c.y, TOL);
    }
}

TYPED_TEST(LnTest, EquivalenceClassUnitPoint) {
    using T = TypeParam;
    constexpr T TOL = static_cast<T>(Eps<T>::VALUE * 5);

    EXPECT_NEAR(ln(static_cast<T>(1.0)), static_cast<T>(0.0), TOL);
}

TYPED_TEST(LnTest, EquivalenceClassPositiveGreaterThanOne) {
    using T = TypeParam;
    constexpr XY<T> CASES[] = {
        {static_cast<T>(1.2), static_cast<T>(0.1823215567939546)},
        {static_cast<T>(2.0), static_cast<T>(0.6931471805599453)},
        {static_cast<T>(10.0), static_cast<T>(2.302585092994046)},
        {static_cast<T>(42.0), static_cast<T>(3.7376696182833684)},
    };
    constexpr T TOL = static_cast<T>(Eps<T>::VALUE * 5);

    for (const auto& c : CASES) {
        const T actual = ln(c.x);
        EXPECT_GT(actual, static_cast<T>(0.0));
        EXPECT_NEAR(actual, c.y, TOL);
    }
}

TYPED_TEST(LnTest, BoundaryZeroAndNearZero) {
    using T = TypeParam;

    EXPECT_TRUE(std::isinf(ln(static_cast<T>(0.0))));
    EXPECT_LT(ln(static_cast<T>(0.0)), static_cast<T>(0.0));

    const T near_zero = std::numeric_limits<T>::epsilon();
    EXPECT_TRUE(std::isfinite(ln(near_zero)));
    EXPECT_LT(ln(near_zero), static_cast<T>(0.0));
}

TYPED_TEST(LnTest, BoundaryNegativeDomain) {
    using T = TypeParam;

    EXPECT_TRUE(std::isnan(ln(static_cast<T>(-1.0))));
    EXPECT_TRUE(std::isnan(ln(static_cast<T>(-0.01))));
}

TYPED_TEST(LnTest, BoundaryNanAndInfinity) {
    using T = TypeParam;
    constexpr auto INF = std::numeric_limits<T>::infinity();
    constexpr auto NaN = std::numeric_limits<T>::quiet_NaN();

    EXPECT_TRUE(std::isnan(ln(NaN)));
    EXPECT_TRUE(std::isinf(ln(INF)));
    EXPECT_GT(ln(INF), static_cast<T>(0.0));
}

TYPED_TEST(LnTest, BoundaryCharacteristicPoints) {
    using T = TypeParam;
    constexpr T X_E = static_cast<T>(2.7182818284590452);
    constexpr T X_E2 = static_cast<T>(7.3890560989306504);
    constexpr T TOL = static_cast<T>(Eps<T>::VALUE * 5);

    EXPECT_NEAR(ln(X_E), static_cast<T>(1.0), TOL);
    EXPECT_NEAR(ln(X_E2), static_cast<T>(2.0), TOL);
}

TEST(LnTestIntegral, EquivalenceClassIntegralOverload) {
    EXPECT_NEAR(ln(1), 0.0f, Eps<float>::VALUE * 5);
    EXPECT_NEAR(ln(2), 0.6931471805599453f, Eps<float>::VALUE * 5);
    EXPECT_NEAR(ln(10), 2.302585092994046f, Eps<float>::VALUE * 5);
}
