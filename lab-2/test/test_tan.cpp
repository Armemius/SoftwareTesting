#include <gtest/gtest.h>
#include <cmath>
#include <cstddef>
#include <limits>
#include <numbers>
#include <type_traits>

#include <maths/cos.hpp>
#include <maths/defaults.hpp>
#include <maths/sin.hpp>
#include <maths/tan.hpp>

using arms::maths::precision::Eps;

template <typename T>
class TanTest : public testing::Test {};

using FloatingPointTypes = testing::Types<float, double, long double>;
TYPED_TEST_SUITE(TanTest, FloatingPointTypes);

template <typename T>
inline constexpr auto SIN_PTR = static_cast<T (*)(T, size_t)>(arms::maths::sin);

template <typename T>
constexpr T cos_from_sin(T x, size_t terms) {
    return arms::maths::cos<T, SIN_PTR<T>>(x, terms);
}

template <typename T>
inline constexpr auto COS_PTR = cos_from_sin<T>;

constexpr auto SIN_FLOAT_PTR = static_cast<float (*)(float, size_t)>(arms::maths::sin);
constexpr auto COS_FLOAT_PTR = cos_from_sin<float>;

TYPED_TEST(TanTest, TestInfinityAndNan) {
    using T = TypeParam;
    constexpr auto INF = std::numeric_limits<T>::infinity();
    constexpr auto NaN = std::numeric_limits<T>::quiet_NaN();

    EXPECT_TRUE((std::isnan(arms::maths::tan<T, SIN_PTR<T>, COS_PTR<T>>(INF))));
    EXPECT_TRUE((std::isnan(arms::maths::tan<T, SIN_PTR<T>, COS_PTR<T>>(-INF))));
    EXPECT_TRUE((std::isnan(arms::maths::tan<T, SIN_PTR<T>, COS_PTR<T>>(NaN))));
}

TYPED_TEST(TanTest, TestDiscontinuityPoints) {
    using T = TypeParam;
    constexpr auto PI = std::numbers::pi_v<T>;

    EXPECT_TRUE((std::isnan(arms::maths::tan<T, SIN_PTR<T>, COS_PTR<T>>(PI / 2))));
    EXPECT_TRUE((std::isnan(arms::maths::tan<T, SIN_PTR<T>, COS_PTR<T>>(-PI / 2))));
    EXPECT_TRUE((std::isnan(arms::maths::tan<T, SIN_PTR<T>, COS_PTR<T>>(3 * PI / 2))));
}

TYPED_TEST(TanTest, TestDiscontinuityNeighborhood) {
    using T = TypeParam;
    constexpr auto PI = std::numbers::pi_v<T>;
    constexpr T DELTA = static_cast<T>(Eps<T>::VALUE * 10);

    EXPECT_FALSE((std::isnan(arms::maths::tan<T, SIN_PTR<T>, COS_PTR<T>>(PI / 2 - DELTA))));
    EXPECT_FALSE((std::isnan(arms::maths::tan<T, SIN_PTR<T>, COS_PTR<T>>(PI / 2 + DELTA))));
}

TYPED_TEST(TanTest, TestZeroAndOddFunction) {
    using T = TypeParam;
    constexpr T X = static_cast<T>(0.7345);

    EXPECT_NEAR((arms::maths::tan<T, SIN_PTR<T>, COS_PTR<T>>(static_cast<T>(0.0))), static_cast<T>(0.0), Eps<T>::VALUE);
    EXPECT_NEAR((arms::maths::tan<T, SIN_PTR<T>, COS_PTR<T>>(-X)),
                -(arms::maths::tan<T, SIN_PTR<T>, COS_PTR<T>>(X)),
                Eps<T>::VALUE);
}

TYPED_TEST(TanTest, TestPeriodicity) {
    using T = TypeParam;
    constexpr T INITIAL = static_cast<T>(0.1459);
    constexpr auto PI = std::numbers::pi_v<T>;

    EXPECT_NEAR((arms::maths::tan<T, SIN_PTR<T>, COS_PTR<T>>(INITIAL)),
                (arms::maths::tan<T, SIN_PTR<T>, COS_PTR<T>>(INITIAL - PI * 30)),
                Eps<T>::VALUE);
    EXPECT_NEAR((arms::maths::tan<T, SIN_PTR<T>, COS_PTR<T>>(INITIAL)),
                (arms::maths::tan<T, SIN_PTR<T>, COS_PTR<T>>(INITIAL + PI * 30)),
                Eps<T>::VALUE);
}

TYPED_TEST(TanTest, TestRangeAgainstStdTan) {
    using T = TypeParam;
    constexpr T VALUES[] = {
        static_cast<T>(-1.3), static_cast<T>(-0.7), static_cast<T>(-0.228),
        static_cast<T>(0.228), static_cast<T>(0.322), static_cast<T>(0.9),
        static_cast<T>(1.2), static_cast<T>(2.28), static_cast<T>(3.22),
        static_cast<T>(4.2), static_cast<T>(5.13)
    };

    for (const T value : VALUES) {
        const auto expected = static_cast<T>(std::tan(static_cast<long double>(value)));
        EXPECT_NEAR((arms::maths::tan<T, SIN_PTR<T>, COS_PTR<T>>(value)), expected, Eps<T>::VALUE * 5);
    }
}

TEST(TanTestIntegral, TestIntegralInput) {
    EXPECT_NEAR((arms::maths::tan<int, SIN_FLOAT_PTR, COS_FLOAT_PTR>(0)), 0.0f, Eps<float>::VALUE);
    EXPECT_NEAR((arms::maths::tan<int, SIN_FLOAT_PTR, COS_FLOAT_PTR>(1)), std::tan(1.0f), Eps<float>::VALUE * 5);
    EXPECT_NEAR((arms::maths::tan<int, SIN_FLOAT_PTR, COS_FLOAT_PTR>(-1)), std::tan(-1.0f), Eps<float>::VALUE * 5);
}
