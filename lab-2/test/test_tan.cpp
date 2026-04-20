#include <gtest/gtest.h>
#include <cmath>
#include <cstddef>
#include <limits>
#include <numbers>
#include <type_traits>

#include "csv_stub_table.hpp"

#include <maths/defaults.hpp>
#include <maths/tan.hpp>

using arms::maths::precision::Eps;
using test::stub::CsvStubTable;

namespace {

CsvStubTable &sin_stub_table() {
    static CsvStubTable table(std::string(STUB_DATA_DIR) + "/sin_for_tan_stub.csv");
    return table;
}

CsvStubTable &cos_stub_table() {
    static CsvStubTable table(std::string(STUB_DATA_DIR) + "/cos_for_tan_stub.csv");
    return table;
}

template <typename T>
T sin_stub_for_tan(T x, size_t) {
    return static_cast<T>(sin_stub_table().lookup(static_cast<double>(x)));
}

template <typename T>
T cos_stub_for_tan(T x, size_t) {
    return static_cast<T>(cos_stub_table().lookup(static_cast<double>(x)));
}

}  // namespace

template <typename T>
class TanTest : public testing::Test {};

using FloatingPointTypes = testing::Types<float, double, long double>;
TYPED_TEST_SUITE(TanTest, FloatingPointTypes);

template <typename T>
inline constexpr auto SIN_PTR = sin_stub_for_tan<T>;

template <typename T>
inline constexpr auto COS_PTR = cos_stub_for_tan<T>;

constexpr auto SIN_FLOAT_PTR = sin_stub_for_tan<float>;
constexpr auto COS_FLOAT_PTR = cos_stub_for_tan<float>;

template <typename T>
struct XY {
    T x;
    T y;
};

TYPED_TEST(TanTest, EquivalenceClassFiniteIntervalsReferenceTable) {
    using T = TypeParam;
    constexpr XY<T> CASES[] = {
        {static_cast<T>(-1.0), static_cast<T>(-0.5)},
        {static_cast<T>(0.0), static_cast<T>(0.0)},
        {static_cast<T>(1.0), static_cast<T>(0.5)},
        {static_cast<T>(2.0), static_cast<T>(6.0)},
    };

    for (const auto &c : CASES) {
        EXPECT_NEAR((arms::maths::tan<T, SIN_PTR<T>, COS_PTR<T>>(c.x)), c.y, Eps<T>::VALUE * 5);
    }
}

TYPED_TEST(TanTest, EquivalenceClassPeriodicity) {
    using T = TypeParam;
    constexpr auto PI = std::numbers::pi_v<T>;

    EXPECT_NEAR((arms::maths::tan<T, SIN_PTR<T>, COS_PTR<T>>(static_cast<T>(0.5))),
                (arms::maths::tan<T, SIN_PTR<T>, COS_PTR<T>>(static_cast<T>(0.5) + PI)), Eps<T>::VALUE);
}

TYPED_TEST(TanTest, EquivalenceClassOddSymmetry) {
    using T = TypeParam;

    EXPECT_NEAR((arms::maths::tan<T, SIN_PTR<T>, COS_PTR<T>>(static_cast<T>(-0.75))),
                -(arms::maths::tan<T, SIN_PTR<T>, COS_PTR<T>>(static_cast<T>(0.75))), Eps<T>::VALUE);
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
    constexpr T LEFT = static_cast<T>(1.5607963267948965);
    constexpr T RIGHT = static_cast<T>(1.5807963267948967);

    EXPECT_FALSE((std::isnan(arms::maths::tan<T, SIN_PTR<T>, COS_PTR<T>>(LEFT))));
    EXPECT_FALSE((std::isnan(arms::maths::tan<T, SIN_PTR<T>, COS_PTR<T>>(RIGHT))));
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
    EXPECT_NEAR((arms::maths::tan<int, SIN_FLOAT_PTR, COS_FLOAT_PTR>(0)), 0.0F, Eps<float>::VALUE);
    EXPECT_NEAR((arms::maths::tan<int, SIN_FLOAT_PTR, COS_FLOAT_PTR>(1)), 0.5F, Eps<float>::VALUE * 5);
    EXPECT_NEAR((arms::maths::tan<int, SIN_FLOAT_PTR, COS_FLOAT_PTR>(-1)), -0.5F, Eps<float>::VALUE * 5);
}
