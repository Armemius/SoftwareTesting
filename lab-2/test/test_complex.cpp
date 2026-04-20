#include <gtest/gtest.h>
#include <cmath>
#include <cstddef>
#include <limits>
#include <type_traits>

#include "csv_stub_table.hpp"

#include <maths/complex.hpp>
#include <maths/defaults.hpp>

using arms::maths::precision::Eps;
using test::stub::CsvStubTable;

namespace {

CsvStubTable &tan_stub_table() {
    static CsvStubTable table(std::string(STUB_DATA_DIR) + "/tan_for_complex_stub.csv");
    return table;
}

CsvStubTable &log5_stub_table() {
    static CsvStubTable table(std::string(STUB_DATA_DIR) + "/log5_for_complex_stub.csv");
    return table;
}

CsvStubTable &ln_stub_table() {
    static CsvStubTable table(std::string(STUB_DATA_DIR) + "/ln_for_complex_stub.csv");
    return table;
}

CsvStubTable &log3_stub_table() {
    static CsvStubTable table(std::string(STUB_DATA_DIR) + "/log3_for_complex_stub.csv");
    return table;
}

template <typename T>
T tan_stub_for_complex(T x, size_t) {
    return static_cast<T>(tan_stub_table().lookup(static_cast<double>(x)));
}

template <typename T>
T log5_stub_for_complex(T x, size_t) {
    return static_cast<T>(log5_stub_table().lookup(static_cast<double>(x)));
}

template <typename T>
T ln_stub_for_complex(T x, size_t) {
    return static_cast<T>(ln_stub_table().lookup(static_cast<double>(x)));
}

template <typename T>
T log3_stub_for_complex(T x, size_t) {
    return static_cast<T>(log3_stub_table().lookup(static_cast<double>(x)));
}

}  // namespace

template <typename T>
class ComplexTest : public testing::Test {};

using FloatingPointTypes = testing::Types<float, double, long double>;
TYPED_TEST_SUITE(ComplexTest, FloatingPointTypes);

template <typename T>
inline constexpr auto TAN_PTR = tan_stub_for_complex<T>;
template <typename T>
inline constexpr auto LOG5_PTR = log5_stub_for_complex<T>;
template <typename T>
inline constexpr auto LN_PTR = ln_stub_for_complex<T>;
template <typename T>
inline constexpr auto LOG3_PTR = log3_stub_for_complex<T>;

constexpr auto TAN_FLOAT_PTR = tan_stub_for_complex<float>;
constexpr auto LOG5_FLOAT_PTR = log5_stub_for_complex<float>;
constexpr auto LN_FLOAT_PTR = ln_stub_for_complex<float>;
constexpr auto LOG3_FLOAT_PTR = log3_stub_for_complex<float>;

template <typename T>
struct XY {
    T x;
    T y;
};

TYPED_TEST(ComplexTest, EquivalenceClassLeftBranchNonPositiveX) {
    using T = TypeParam;
    constexpr XY<T> CASES[] = {
        {static_cast<T>(-2.0), static_cast<T>(-7.0)},
        {static_cast<T>(-0.5), static_cast<T>(-1.5)},
        {static_cast<T>(0.0), static_cast<T>(0.25)},
    };

    for (const auto &c : CASES) {
        const T actual = arms::maths::complex<T, TAN_PTR<T>, LOG5_PTR<T>, LN_PTR<T>, LOG3_PTR<T>>(c.x);
        EXPECT_NEAR(actual, c.y, Eps<T>::VALUE);
    }
}

TYPED_TEST(ComplexTest, EquivalenceClassRightBranchPositiveXLessThanOne) {
    using T = TypeParam;
    constexpr XY<T> CASES[] = {
        {static_cast<T>(0.5), static_cast<T>(2.0583673469387755)},
    };
    constexpr T TOL = static_cast<T>(Eps<T>::VALUE * 20);

    for (const auto &c : CASES) {
        const T actual = arms::maths::complex<T, TAN_PTR<T>, LOG5_PTR<T>, LN_PTR<T>, LOG3_PTR<T>>(c.x);
        EXPECT_NEAR(actual, c.y, TOL);
    }
}

TYPED_TEST(ComplexTest, EquivalenceClassRightBranchPositiveXGreaterThanOne) {
    using T = TypeParam;
    constexpr XY<T> CASES[] = {
        {static_cast<T>(2.0), static_cast<T>(6.797819492680379e-05)},
        {static_cast<T>(5.0), static_cast<T>(1.0274355640600286)},
    };
    constexpr T TOL = static_cast<T>(Eps<T>::VALUE * 20);

    for (const auto &c : CASES) {
        const T actual = arms::maths::complex<T, TAN_PTR<T>, LOG5_PTR<T>, LN_PTR<T>, LOG3_PTR<T>>(c.x);
        EXPECT_NEAR(actual, c.y, TOL);
    }
}

TYPED_TEST(ComplexTest, BoundaryLeftBranchDiscontinuityPoint) {
    using T = TypeParam;

    EXPECT_TRUE((std::isnan(arms::maths::complex<T, TAN_PTR<T>, LOG5_PTR<T>, LN_PTR<T>, LOG3_PTR<T>>(
        static_cast<T>(-1.5707963267948966)))));
}

TYPED_TEST(ComplexTest, BoundaryRightBranchSingularityAtOne) {
    using T = TypeParam;

    EXPECT_TRUE(
        (std::isnan(arms::maths::complex<T, TAN_PTR<T>, LOG5_PTR<T>, LN_PTR<T>, LOG3_PTR<T>>(static_cast<T>(1.0)))));
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
    EXPECT_NEAR((arms::maths::complex<int, TAN_FLOAT_PTR, LOG5_FLOAT_PTR, LN_FLOAT_PTR, LOG3_FLOAT_PTR>(0)), 0.25f,
                Eps<float>::VALUE);
    EXPECT_NEAR((arms::maths::complex<int, TAN_FLOAT_PTR, LOG5_FLOAT_PTR, LN_FLOAT_PTR, LOG3_FLOAT_PTR>(5)),
                1.0274355640600286F, Eps<float>::VALUE * 20);
}
