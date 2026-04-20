#include <gtest/gtest.h>
#include <cmath>
#include <limits>
#include <numbers>
#include <type_traits>

#include <maths/defaults.hpp>
#include <maths/ln.hpp>

using arms::maths::ln;
using arms::maths::precision::Eps;

template <typename T>
class LnTest : public testing::Test {};

using FloatingPointTypes = testing::Types<float, double, long double>;
TYPED_TEST_SUITE(LnTest, FloatingPointTypes);

TYPED_TEST(LnTest, TestDomainAndSpecialValues) {
    using T = TypeParam;
    constexpr auto INF = std::numeric_limits<T>::infinity();
    constexpr auto NaN = std::numeric_limits<T>::quiet_NaN();

    EXPECT_TRUE(std::isnan(ln(static_cast<T>(-1.0))));
    EXPECT_TRUE(std::isnan(ln(NaN)));
    EXPECT_TRUE(std::isinf(ln(static_cast<T>(0.0))));
    EXPECT_LT(ln(static_cast<T>(0.0)), static_cast<T>(0.0));
    EXPECT_TRUE(std::isinf(ln(INF)));
    EXPECT_GT(ln(INF), static_cast<T>(0.0));
}

TYPED_TEST(LnTest, TestCharacteristicPoints) {
    using T = TypeParam;
    constexpr auto E = std::numbers::e_v<T>;
    constexpr T TOL = static_cast<T>(Eps<T>::VALUE * 5);

    EXPECT_NEAR(ln(static_cast<T>(1.0)), static_cast<T>(0.0), TOL);
    EXPECT_NEAR(ln(E), static_cast<T>(1.0), TOL);
    EXPECT_NEAR(ln(E * E), static_cast<T>(2.0), TOL);
}

TYPED_TEST(LnTest, TestRangeAgainstStdLog) {
    using T = TypeParam;
    constexpr T VALUES[] = {
        static_cast<T>(0.1),
        static_cast<T>(0.25),
        static_cast<T>(0.5),
        static_cast<T>(0.75),
        static_cast<T>(1.2),
        static_cast<T>(2.0),
        static_cast<T>(10.0),
        static_cast<T>(42.0)
    };
    constexpr T TOL = static_cast<T>(Eps<T>::VALUE * 5);

    for (const T value : VALUES) {
        const T expected = static_cast<T>(std::log(static_cast<long double>(value)));
        EXPECT_NEAR(ln(value), expected, TOL);
    }
}

TEST(LnTestIntegral, TestIntegralInput) {
    EXPECT_NEAR(ln(1), 0.0f, Eps<float>::VALUE * 5);
    EXPECT_NEAR(ln(2), std::log(2.0f), Eps<float>::VALUE * 5);
    EXPECT_NEAR(ln(10), std::log(10.0f), Eps<float>::VALUE * 5);
}
