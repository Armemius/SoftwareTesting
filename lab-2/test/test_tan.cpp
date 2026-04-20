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

template <typename T>
struct XY {
    T x;
    T y;
};

TYPED_TEST(TanTest, EquivalenceClassFiniteIntervalsReferenceTable) {
    using T = TypeParam;
    constexpr XY<T> CASES[] = {
        {static_cast<T>(-1.3), static_cast<T>(-3.6021024479679786)},
        {static_cast<T>(-0.7), static_cast<T>(-0.8422883804630794)},
        {static_cast<T>(-0.228), static_cast<T>(-0.2320347007698748)},
        {static_cast<T>(0.228), static_cast<T>(0.2320347007698748)},
        {static_cast<T>(0.322), static_cast<T>(0.3336105181681039)},
        {static_cast<T>(0.9), static_cast<T>(1.2601582175503392)},
        {static_cast<T>(1.2), static_cast<T>(2.5721516221263188)},
        {static_cast<T>(2.28), static_cast<T>(-1.1653042761691337)},
        {static_cast<T>(3.22), static_cast<T>(0.07856841777065368)},
        {static_cast<T>(4.2), static_cast<T>(1.7777797745088417)},
        {static_cast<T>(5.13), static_cast<T>(-2.2537233301228943)},
    };

    for (const auto& c : CASES) {
        EXPECT_NEAR((arms::maths::tan<T, SIN_PTR<T>, COS_PTR<T>>(c.x)), c.y, Eps<T>::VALUE * 5);
    }
}

TYPED_TEST(TanTest, EquivalenceClassPeriodicity) {
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

TYPED_TEST(TanTest, EquivalenceClassOddSymmetry) {
    using T = TypeParam;
    constexpr T X = static_cast<T>(0.7345);

    EXPECT_NEAR((arms::maths::tan<T, SIN_PTR<T>, COS_PTR<T>>(-X)),
                -(arms::maths::tan<T, SIN_PTR<T>, COS_PTR<T>>(X)),
                Eps<T>::VALUE);
}

TYPED_TEST(TanTest, BoundaryDiscontinuityPoints) {
    using T = TypeParam;
    constexpr auto PI = std::numbers::pi_v<T>;

    EXPECT_TRUE((std::isnan(arms::maths::tan<T, SIN_PTR<T>, COS_PTR<T>>(PI / 2))));
    EXPECT_TRUE((std::isnan(arms::maths::tan<T, SIN_PTR<T>, COS_PTR<T>>(-PI / 2))));
    EXPECT_TRUE((std::isnan(arms::maths::tan<T, SIN_PTR<T>, COS_PTR<T>>(3 * PI / 2))));
}

TYPED_TEST(TanTest, BoundaryDiscontinuityNeighborhood) {
    using T = TypeParam;
    constexpr auto PI = std::numbers::pi_v<T>;
    constexpr T DELTA = static_cast<T>(Eps<T>::VALUE * 10);

    EXPECT_FALSE((std::isnan(arms::maths::tan<T, SIN_PTR<T>, COS_PTR<T>>(PI / 2 - DELTA))));
    EXPECT_FALSE((std::isnan(arms::maths::tan<T, SIN_PTR<T>, COS_PTR<T>>(PI / 2 + DELTA))));
}

TYPED_TEST(TanTest, BoundaryZero) {
    using T = TypeParam;

    EXPECT_NEAR((arms::maths::tan<T, SIN_PTR<T>, COS_PTR<T>>(static_cast<T>(0.0))), static_cast<T>(0.0), Eps<T>::VALUE);
}

TYPED_TEST(TanTest, BoundaryNanAndInfinity) {
    using T = TypeParam;
    constexpr auto INF = std::numeric_limits<T>::infinity();
    constexpr auto NaN = std::numeric_limits<T>::quiet_NaN();

    EXPECT_TRUE((std::isnan(arms::maths::tan<T, SIN_PTR<T>, COS_PTR<T>>(INF))));
    EXPECT_TRUE((std::isnan(arms::maths::tan<T, SIN_PTR<T>, COS_PTR<T>>(-INF))));
    EXPECT_TRUE((std::isnan(arms::maths::tan<T, SIN_PTR<T>, COS_PTR<T>>(NaN))));
}

TEST(TanTestIntegral, EquivalenceClassIntegralOverload) {
    EXPECT_NEAR((arms::maths::tan<int, SIN_FLOAT_PTR, COS_FLOAT_PTR>(0)), 0.0f, Eps<float>::VALUE);
    EXPECT_NEAR((arms::maths::tan<int, SIN_FLOAT_PTR, COS_FLOAT_PTR>(1)), 1.5574077246549023f, Eps<float>::VALUE * 5);
    EXPECT_NEAR((arms::maths::tan<int, SIN_FLOAT_PTR, COS_FLOAT_PTR>(-1)), -1.5574077246549023f, Eps<float>::VALUE * 5);
}
