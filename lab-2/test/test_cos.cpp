#include <gtest/gtest.h>
#include <cmath>
#include <cstddef>
#include <limits>
#include <numbers>
#include <type_traits>

#include "csv_stub_table.hpp"

#include <maths/cos.hpp>
#include <maths/defaults.hpp>

using arms::maths::precision::Eps;
using test::stub::CsvStubTable;

namespace {

CsvStubTable &sin_stub_table() {
    static CsvStubTable table(std::string(STUB_DATA_DIR) + "/sin_for_cos_stub.csv");
    return table;
}

template <typename T>
T sin_stub_for_cos(T x, size_t) {
    return static_cast<T>(sin_stub_table().lookup(static_cast<double>(x)));
}

}  // namespace

template <typename T>
class CosTest : public testing::Test {};

using FloatingPointTypes = testing::Types<float, double, long double>;
TYPED_TEST_SUITE(CosTest, FloatingPointTypes);

template <typename T>
inline constexpr auto SIN_PTR = sin_stub_for_cos<T>;

constexpr auto SIN_FLOAT_PTR = sin_stub_for_cos<float>;

template <typename T>
struct XY {
    T x;
    T y;
};

TYPED_TEST(CosTest, EquivalenceClassFiniteValuesReferenceTable) {
    using T = TypeParam;
    constexpr XY<T> CASES[] = {
        {static_cast<T>(-1.0), static_cast<T>(1.234)},
        {static_cast<T>(0.0), static_cast<T>(0.321)},
        {static_cast<T>(1.0), static_cast<T>(-0.654)},
    };

    for (const auto &c : CASES) {
        EXPECT_NEAR((arms::maths::cos<T, SIN_PTR<T>>(c.x)), c.y, Eps<T>::VALUE);
    }
}

TYPED_TEST(CosTest, EquivalenceClassPeriodicity) {
    using T = TypeParam;
    constexpr auto TWO_PI = static_cast<T>(2) * std::numbers::pi_v<T>;

    EXPECT_NEAR((arms::maths::cos<T, SIN_PTR<T>>(static_cast<T>(0.0))), (arms::maths::cos<T, SIN_PTR<T>>(TWO_PI)),
                Eps<T>::VALUE);
}

TYPED_TEST(CosTest, EquivalenceClassEvenSymmetry) {
    using T = TypeParam;

    EXPECT_NEAR((arms::maths::cos<T, SIN_PTR<T>>(static_cast<T>(0.0))),
                (arms::maths::cos<T, SIN_PTR<T>>(static_cast<T>(-0.0))), Eps<T>::VALUE);
}

TYPED_TEST(CosTest, BoundaryCharacteristicPoints) {
    using T = TypeParam;
    constexpr auto PI = std::numbers::pi_v<T>;

    EXPECT_NEAR((arms::maths::cos<T, SIN_PTR<T>>(static_cast<T>(0.0))), static_cast<T>(0.321), Eps<T>::VALUE);
    EXPECT_NEAR((arms::maths::cos<T, SIN_PTR<T>>(PI / 2)), static_cast<T>(0.0), Eps<T>::VALUE);
    EXPECT_NEAR((arms::maths::cos<T, SIN_PTR<T>>(PI)), static_cast<T>(-0.7), Eps<T>::VALUE);
    EXPECT_NEAR((arms::maths::cos<T, SIN_PTR<T>>(static_cast<T>(2) * PI)), static_cast<T>(0.321), Eps<T>::VALUE);
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
    EXPECT_NEAR((arms::maths::cos<int, SIN_FLOAT_PTR>(0)), 0.321F, Eps<float>::VALUE);
    EXPECT_NEAR((arms::maths::cos<int, SIN_FLOAT_PTR>(1)), -0.654F, Eps<float>::VALUE);
    EXPECT_NEAR((arms::maths::cos<int, SIN_FLOAT_PTR>(-1)), 1.234F, Eps<float>::VALUE);
}
