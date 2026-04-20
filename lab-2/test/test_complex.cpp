#include <gtest/gtest.h>
#include <cmath>
#include <cstddef>
#include <limits>
#include <numbers>
#include <type_traits>

#include <maths/complex.hpp>
#include <maths/cos.hpp>
#include <maths/defaults.hpp>
#include <maths/ln.hpp>
#include <maths/log.hpp>
#include <maths/sin.hpp>
#include <maths/tan.hpp>

using arms::maths::precision::Eps;

template <typename T>
class ComplexTest : public testing::Test {};

using FloatingPointTypes = testing::Types<float, double, long double>;
TYPED_TEST_SUITE(ComplexTest, FloatingPointTypes);

template <typename T>
inline constexpr auto SIN_PTR = static_cast<T (*)(T, size_t)>(arms::maths::sin);

template <typename T>
inline constexpr auto LN_PTR = static_cast<T (*)(T, size_t)>(arms::maths::ln);

template <typename T>
constexpr T cos_from_sin(T x, size_t terms) {
    return arms::maths::cos<T, SIN_PTR<T>>(x, terms);
}

template <typename T>
constexpr T tan_from_sin_cos(T x, size_t terms) {
    return arms::maths::tan<T, SIN_PTR<T>, cos_from_sin<T>>(x, terms);
}

template <typename T>
constexpr T log3_from_ln(T x, size_t terms) {
    return arms::maths::log_3<T, LN_PTR<T>>(x, terms);
}

template <typename T>
constexpr T log5_from_ln(T x, size_t terms) {
    return arms::maths::log_5<T, LN_PTR<T>>(x, terms);
}

template <typename T>
inline constexpr auto TAN_PTR = tan_from_sin_cos<T>;

template <typename T>
inline constexpr auto LOG3_PTR = log3_from_ln<T>;

template <typename T>
inline constexpr auto LOG5_PTR = log5_from_ln<T>;

constexpr auto SIN_FLOAT_PTR = static_cast<float (*)(float, size_t)>(arms::maths::sin);
constexpr auto LN_FLOAT_PTR = static_cast<float (*)(float, size_t)>(arms::maths::ln);
constexpr auto TAN_FLOAT_PTR = tan_from_sin_cos<float>;
constexpr auto LOG3_FLOAT_PTR = log3_from_ln<float>;
constexpr auto LOG5_FLOAT_PTR = log5_from_ln<float>;

template <typename T>
struct XY {
    T x;
    T y;
};

TYPED_TEST(ComplexTest, EquivalenceClassLeftBranchNonPositiveX) {
    using T = TypeParam;
    constexpr T VALUES[] = {
        static_cast<T>(0.0),
        static_cast<T>(-0.2),
        static_cast<T>(-0.7),
        static_cast<T>(-1.3),
        static_cast<T>(-3.2),
    };

    for (const T value : VALUES) {
        const T expected = tan_from_sin_cos<T>(value, arms::maths::defaults::DEFAULT_TERMS<T>);
        const T actual = arms::maths::complex<T, TAN_PTR<T>, LOG5_PTR<T>, LN_PTR<T>, LOG3_PTR<T>>(value);
        if (std::isnan(expected)) {
            EXPECT_TRUE(std::isnan(actual));
        } else {
            EXPECT_NEAR(actual, expected, Eps<T>::VALUE);
        }
    }
}

TYPED_TEST(ComplexTest, EquivalenceClassRightBranchPositiveXLessThanOne) {
    using T = TypeParam;
    constexpr XY<T> CASES[] = {
        {static_cast<T>(0.15), static_cast<T>(8.077706918393678)},
        {static_cast<T>(0.25), static_cast<T>(5.195413305514769)},
        {static_cast<T>(0.5), static_cast<T>(2.297322910057771)},
        {static_cast<T>(0.75), static_cast<T>(1.1427268433179338)},
    };
    constexpr T TOL = static_cast<T>(Eps<T>::VALUE * 20);

    for (const auto& c : CASES) {
        const T actual = arms::maths::complex<T, TAN_PTR<T>, LOG5_PTR<T>, LN_PTR<T>, LOG3_PTR<T>>(c.x);
        EXPECT_NEAR(actual, c.y, TOL);
    }
}

TYPED_TEST(ComplexTest, EquivalenceClassRightBranchPositiveXGreaterThanOne) {
    using T = TypeParam;
    constexpr XY<T> CASES[] = {
        {static_cast<T>(1.2), static_cast<T>(0.30379121027999106)},
        {static_cast<T>(2.0), static_cast<T>(0.00013486627079233612)},
        {static_cast<T>(3.0), static_cast<T>(0.21005849777402996)},
        {static_cast<T>(5.0), static_cast<T>(1.04265983784566)},
        {static_cast<T>(10.0), static_cast<T>(3.1853677949271244)},
        {static_cast<T>(42.0), static_cast<T>(11.328732737421294)},
    };
    constexpr T TOL = static_cast<T>(Eps<T>::VALUE * 20);

    for (const auto& c : CASES) {
        const T actual = arms::maths::complex<T, TAN_PTR<T>, LOG5_PTR<T>, LN_PTR<T>, LOG3_PTR<T>>(c.x);
        EXPECT_NEAR(actual, c.y, TOL);
    }
}

TYPED_TEST(ComplexTest, BoundaryLeftBranchDiscontinuityPoint) {
    using T = TypeParam;
    constexpr auto PI = std::numbers::pi_v<T>;

    EXPECT_TRUE((std::isnan(arms::maths::complex<T, TAN_PTR<T>, LOG5_PTR<T>, LN_PTR<T>, LOG3_PTR<T>>(-PI / 2))));
}

TYPED_TEST(ComplexTest, BoundaryRightBranchSingularityAtOne) {
    using T = TypeParam;

    EXPECT_TRUE((std::isnan(arms::maths::complex<T, TAN_PTR<T>, LOG5_PTR<T>, LN_PTR<T>, LOG3_PTR<T>>(static_cast<T>(1.0)))));
}

TYPED_TEST(ComplexTest, BoundaryNanAndInfinity) {
    using T = TypeParam;
    constexpr auto INF = std::numeric_limits<T>::infinity();
    constexpr auto NaN = std::numeric_limits<T>::quiet_NaN();

    EXPECT_TRUE((std::isnan(arms::maths::complex<T, TAN_PTR<T>, LOG5_PTR<T>, LN_PTR<T>, LOG3_PTR<T>>(NaN))));
    EXPECT_TRUE((std::isnan(arms::maths::complex<T, TAN_PTR<T>, LOG5_PTR<T>, LN_PTR<T>, LOG3_PTR<T>>(INF))));
    EXPECT_TRUE((std::isnan(arms::maths::complex<T, TAN_PTR<T>, LOG5_PTR<T>, LN_PTR<T>, LOG3_PTR<T>>(-INF))));
}

TEST(ComplexTestIntegral, EquivalenceClassIntegralOverload) {
    EXPECT_NEAR((arms::maths::complex<int, TAN_FLOAT_PTR, LOG5_FLOAT_PTR, LN_FLOAT_PTR, LOG3_FLOAT_PTR>(0)),
                0.0f,
                Eps<float>::VALUE);
    EXPECT_NEAR((arms::maths::complex<int, TAN_FLOAT_PTR, LOG5_FLOAT_PTR, LN_FLOAT_PTR, LOG3_FLOAT_PTR>(5)),
                1.04265983784566f,
                Eps<float>::VALUE * 20);
}
