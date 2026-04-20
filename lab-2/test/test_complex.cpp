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

TYPED_TEST(ComplexTest, TestLeftBranchEqualsTanForNonPositiveX) {
    using T = TypeParam;
    constexpr T VALUES[] = {static_cast<T>(0.0), static_cast<T>(-0.2), static_cast<T>(-0.7), static_cast<T>(-1.3),
                            static_cast<T>(-3.2)};

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

TYPED_TEST(ComplexTest, TestLeftBranchDiscontinuityPoint) {
    using T = TypeParam;
    constexpr auto PI = std::numbers::pi_v<T>;

    EXPECT_TRUE((std::isnan(arms::maths::complex<T, TAN_PTR<T>, LOG5_PTR<T>, LN_PTR<T>, LOG3_PTR<T>>(-PI / 2))));
}

TYPED_TEST(ComplexTest, TestRightBranchAgainstReferenceFormula) {
    using T = TypeParam;
    constexpr T VALUES[] = {static_cast<T>(0.25), static_cast<T>(0.5),  static_cast<T>(0.75),
                            static_cast<T>(1.2),  static_cast<T>(2.0),  static_cast<T>(3.0),
                            static_cast<T>(5.0),  static_cast<T>(10.0), static_cast<T>(42.0)};
    constexpr T TOL = static_cast<T>(Eps<T>::VALUE * 20);

    for (const T value : VALUES) {
        const long double x = static_cast<long double>(value);
        const long double log5 = std::log(x) / std::log(static_cast<long double>(5.0));
        const long double ln = std::log(x);
        const long double log3 = std::log(x) / std::log(static_cast<long double>(3.0));
        const long double diff = log5 / ln - log3;
        const T expected = static_cast<T>((diff * diff / log5) * log3);

        const T actual = arms::maths::complex<T, TAN_PTR<T>, LOG5_PTR<T>, LN_PTR<T>, LOG3_PTR<T>>(value);
        EXPECT_NEAR(actual, expected, TOL);
    }
}

TYPED_TEST(ComplexTest, TestRightBranchSingularityAtOne) {
    using T = TypeParam;

    EXPECT_TRUE(
        (std::isnan(arms::maths::complex<T, TAN_PTR<T>, LOG5_PTR<T>, LN_PTR<T>, LOG3_PTR<T>>(static_cast<T>(1.0)))));
}

TYPED_TEST(ComplexTest, TestNanInput) {
    using T = TypeParam;
    constexpr auto NaN = std::numeric_limits<T>::quiet_NaN();

    EXPECT_TRUE((std::isnan(arms::maths::complex<T, TAN_PTR<T>, LOG5_PTR<T>, LN_PTR<T>, LOG3_PTR<T>>(NaN))));
}

TEST(ComplexTestIntegral, TestIntegralInput) {
    EXPECT_NEAR((arms::maths::complex<int, TAN_FLOAT_PTR, LOG5_FLOAT_PTR, LN_FLOAT_PTR, LOG3_FLOAT_PTR>(0)), 0.0f,
                Eps<float>::VALUE);

    const float x = 5.0f;
    const float log5 = std::log(x) / std::log(5.0f);
    const float ln = std::log(x);
    const float log3 = std::log(x) / std::log(3.0f);
    const float diff = log5 / ln - log3;
    const float expected = (diff * diff / log5) * log3;
    EXPECT_NEAR((arms::maths::complex<int, TAN_FLOAT_PTR, LOG5_FLOAT_PTR, LN_FLOAT_PTR, LOG3_FLOAT_PTR>(5)), expected,
                Eps<float>::VALUE * 20);
}
