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

template <typename T>
struct XY {
    T x;
    T y;
};

TYPED_TEST(CosTest, EquivalenceClassFiniteValuesReferenceTable) {
    using T = TypeParam;
    constexpr XY<T> CASES[] = {
        {static_cast<T>(-4.8), static_cast<T>(0.0874989834394464)},
        {static_cast<T>(-2.1), static_cast<T>(-0.5048461045998576)},
        {static_cast<T>(-0.8), static_cast<T>(0.6967067093471654)},
        {static_cast<T>(-0.1), static_cast<T>(0.9950041652780258)},
        {static_cast<T>(0.1), static_cast<T>(0.9950041652780258)},
        {static_cast<T>(0.8), static_cast<T>(0.6967067093471654)},
        {static_cast<T>(2.1), static_cast<T>(-0.5048461045998576)},
        {static_cast<T>(4.8), static_cast<T>(0.0874989834394464)},
    };

    for (const auto& c : CASES) {
        EXPECT_NEAR((arms::maths::cos<T, SIN_PTR<T>>(c.x)), c.y, Eps<T>::VALUE);
    }
}

TYPED_TEST(CosTest, EquivalenceClassPeriodicity) {
    using T = TypeParam;
    constexpr T X = static_cast<T>(0.2357);
    constexpr auto PI = std::numbers::pi_v<T>;
    constexpr auto PERIOD = static_cast<T>(2) * PI;

    EXPECT_NEAR((arms::maths::cos<T, SIN_PTR<T>>(X)), (arms::maths::cos<T, SIN_PTR<T>>(X + PERIOD * 10)), Eps<T>::VALUE);
    EXPECT_NEAR((arms::maths::cos<T, SIN_PTR<T>>(X)), (arms::maths::cos<T, SIN_PTR<T>>(X - PERIOD * 10)), Eps<T>::VALUE);
}

TYPED_TEST(CosTest, EquivalenceClassEvenSymmetry) {
    using T = TypeParam;
    constexpr T X = static_cast<T>(0.7345);

    EXPECT_NEAR((arms::maths::cos<T, SIN_PTR<T>>(X)), (arms::maths::cos<T, SIN_PTR<T>>(-X)), Eps<T>::VALUE);
}

TYPED_TEST(CosTest, BoundaryCharacteristicPoints) {
    using T = TypeParam;
    constexpr auto PI = std::numbers::pi_v<T>;

    EXPECT_NEAR((arms::maths::cos<T, SIN_PTR<T>>(static_cast<T>(0.0))), static_cast<T>(1.0), Eps<T>::VALUE);
    EXPECT_NEAR((arms::maths::cos<T, SIN_PTR<T>>(PI / 2)), static_cast<T>(0.0), Eps<T>::VALUE);
    EXPECT_NEAR((arms::maths::cos<T, SIN_PTR<T>>(PI)), static_cast<T>(-1.0), Eps<T>::VALUE);
    EXPECT_NEAR((arms::maths::cos<T, SIN_PTR<T>>(static_cast<T>(2) * PI)), static_cast<T>(1.0), Eps<T>::VALUE);
}

TYPED_TEST(CosTest, BoundaryNanAndInfinity) {
    using T = TypeParam;
    constexpr auto INF = std::numeric_limits<T>::infinity();
    constexpr auto NaN = std::numeric_limits<T>::quiet_NaN();

    EXPECT_TRUE((std::isnan(arms::maths::cos<T, SIN_PTR<T>>(INF))));
    EXPECT_TRUE((std::isnan(arms::maths::cos<T, SIN_PTR<T>>(-INF))));
    EXPECT_TRUE((std::isnan(arms::maths::cos<T, SIN_PTR<T>>(NaN))));
}

TEST(CosTestIntegral, EquivalenceClassIntegralOverload) {
    EXPECT_NEAR((arms::maths::cos<int, SIN_FLOAT_PTR>(0)), 1.0f, Eps<float>::VALUE);
    EXPECT_NEAR((arms::maths::cos<int, SIN_FLOAT_PTR>(1)), 0.5403023058681398f, Eps<float>::VALUE);
    EXPECT_NEAR((arms::maths::cos<int, SIN_FLOAT_PTR>(-1)), 0.5403023058681398f, Eps<float>::VALUE);
}
