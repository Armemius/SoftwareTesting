#include <gtest/gtest.h>
#include <cmath>
#include <numbers>
#include <type_traits>

#include <maths/maths.hpp>
#include <maths/defaults.hpp>

using arms::maths::sec;
using arms::maths::precision::Eps;

template <typename T>
class SecTest : public testing::Test {};

using FloatingPointTypes = testing::Types<float, double, long double>;
TYPED_TEST_SUITE(SecTest, FloatingPointTypes);

TYPED_TEST(SecTest, TestPointsOfDiscontinuity) {
    using T = TypeParam;
    constexpr auto PI = std::numbers::pi_v<T>;

    EXPECT_TRUE(std::isnan(sec(-PI / 2)));
    EXPECT_TRUE(std::isnan(sec(PI / 2)));
    EXPECT_TRUE(std::isnan(sec(3 * PI / 2)));
}

TYPED_TEST(SecTest, TestDiscontinuityNeighborhoodRegion) {
    using T = TypeParam;
    constexpr auto PI = std::numbers::pi_v<T>;

    EXPECT_FALSE(std::isnan(sec(-PI / 2 - Eps<T>::VALUE)));
    EXPECT_FALSE(std::isnan(sec(-PI / 2 + Eps<T>::VALUE)));
    EXPECT_FALSE(std::isnan(sec(PI / 2 - Eps<T>::VALUE)));
    EXPECT_FALSE(std::isnan(sec(PI / 2 + Eps<T>::VALUE)));
}

TYPED_TEST(SecTest, TestInfinity) {
    using T = TypeParam;
    constexpr auto INF = std::numeric_limits<T>::infinity();

    EXPECT_TRUE(std::isnan(sec(INF)));
    EXPECT_TRUE(std::isnan(sec(-INF)));
}

TYPED_TEST(SecTest, TestZero) {
    using T = TypeParam;

    EXPECT_NEAR(sec(static_cast<T>(0.0)), static_cast<T>(1.0), Eps<T>::VALUE);
}

TYPED_TEST(SecTest, TestRange1) {
    using T = TypeParam;

    EXPECT_NEAR(sec(static_cast<T>(0.228)), static_cast<T>(1.02656714459), Eps<T>::VALUE);
    EXPECT_NEAR(sec(static_cast<T>(0.322)), static_cast<T>(1.05418023973), Eps<T>::VALUE);
    EXPECT_NEAR(sec(static_cast<T>(1.337)), static_cast<T>(4.31644314933), Eps<T>::VALUE);
}

TYPED_TEST(SecTest, TestRange2) {
    using T = TypeParam;

    EXPECT_NEAR(sec(static_cast<T>(1.7)), static_cast<T>(-7.76129399605), Eps<T>::VALUE);
    EXPECT_NEAR(sec(static_cast<T>(1.812)), static_cast<T>(-4.18634900555), Eps<T>::VALUE);
    EXPECT_NEAR(sec(static_cast<T>(2.28)), static_cast<T>(-1.53555659487), Eps<T>::VALUE);
}

TYPED_TEST(SecTest, TestRange3) {
    using T = TypeParam;

    EXPECT_NEAR(sec(static_cast<T>(3.22)), static_cast<T>(-1.00308174955), Eps<T>::VALUE);
    EXPECT_NEAR(sec(static_cast<T>(4.2)), static_cast<T>(-2.03973060149), Eps<T>::VALUE);
    EXPECT_NEAR(sec(static_cast<T>(4.6)), static_cast<T>(-8.91642861136), Eps<T>::VALUE);
}

TYPED_TEST(SecTest, TestRange4) {
    using T = TypeParam;

    EXPECT_NEAR(sec(static_cast<T>(4.88)), static_cast<T>(5.99422183928), Eps<T>::VALUE);
    EXPECT_NEAR(sec(static_cast<T>(5)), static_cast<T>(3.52532008582), Eps<T>::VALUE);
    EXPECT_NEAR(sec(static_cast<T>(5.13)), static_cast<T>(2.46561733623), Eps<T>::VALUE);
}

TYPED_TEST(SecTest, TestExtremum) {
    using T = TypeParam;
    constexpr auto PI = std::numbers::pi_v<T>;

    EXPECT_NEAR(sec(-PI), -1.0, Eps<T>::VALUE);
    EXPECT_NEAR(sec<T>(0.0), 1.0, Eps<T>::VALUE);
    EXPECT_NEAR(sec(PI), -1.0, Eps<T>::VALUE);
}

TYPED_TEST(SecTest, TestIntervals) {
    using T = TypeParam;
    constexpr T INITIAL = 0.1459;
    constexpr auto PI = std::numbers::pi_v<T>;
    constexpr auto INTERVAL = 2 * PI;

    EXPECT_NEAR(sec(INITIAL), sec(INITIAL - INTERVAL * 30), Eps<T>::VALUE);
    EXPECT_NEAR(sec(INITIAL), sec(INITIAL - INTERVAL * 20), Eps<T>::VALUE);
    EXPECT_NEAR(sec(INITIAL), sec(INITIAL - INTERVAL * 10), Eps<T>::VALUE);
    EXPECT_NEAR(sec(INITIAL), sec(INITIAL + INTERVAL * 10), Eps<T>::VALUE);
    EXPECT_NEAR(sec(INITIAL), sec(INITIAL + INTERVAL * 20), Eps<T>::VALUE);
    EXPECT_NEAR(sec(INITIAL), sec(INITIAL + INTERVAL * 30), Eps<T>::VALUE);
}
