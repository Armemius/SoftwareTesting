#include <gtest/gtest.h>
#include <cmath>
#include <cstddef>
#include <limits>
#include <type_traits>

#include "csv_stub_table.hpp"

#include <maths/defaults.hpp>
#include <maths/log.hpp>

using arms::maths::precision::Eps;
using test::stub::CsvStubTable;

namespace {

CsvStubTable &ln_stub_table() {
    static CsvStubTable table(std::string(STUB_DATA_DIR) + "/ln_for_log_stub.csv");
    return table;
}

template <typename T>
T ln_stub_for_log(T x, size_t) {
    return static_cast<T>(ln_stub_table().lookup(static_cast<double>(x)));
}

}  // namespace

template <typename T>
class LogTest : public testing::Test {};

using FloatingPointTypes = testing::Types<float, double, long double>;
TYPED_TEST_SUITE(LogTest, FloatingPointTypes);

template <typename T>
inline constexpr auto LN_PTR = ln_stub_for_log<T>;

constexpr auto LN_FLOAT_PTR = ln_stub_for_log<float>;

template <typename T>
struct XYZ {
    T x;
    T log3;
    T log5;
};

TYPED_TEST(LogTest, EquivalenceClassPositiveLessThanOne) {
    using T = TypeParam;
    constexpr XYZ<T> CASES[] = {
        {static_cast<T>(0.1), static_cast<T>(-2.0959032742893844), static_cast<T>(-1.430676558073393)},
        {static_cast<T>(0.25), static_cast<T>(-1.2618595071429148), static_cast<T>(-0.8613531161467861)},
        {static_cast<T>(0.5), static_cast<T>(-0.6309297535714574), static_cast<T>(-0.43067655807339306)},
        {static_cast<T>(0.75), static_cast<T>(-0.26185950714291484), static_cast<T>(-0.1787469216608008)},
    };
    constexpr T TOL = static_cast<T>(Eps<T>::VALUE * 5);

    for (const auto &c : CASES) {
        const T log3_actual = arms::maths::log_3<T, LN_PTR<T>>(c.x);
        const T log5_actual = arms::maths::log_5<T, LN_PTR<T>>(c.x);

        EXPECT_LT(log3_actual, static_cast<T>(0.0));
        EXPECT_LT(log5_actual, static_cast<T>(0.0));
        EXPECT_NEAR(log3_actual, c.log3, TOL);
        EXPECT_NEAR(log5_actual, c.log5, TOL);
    }
}

TYPED_TEST(LogTest, EquivalenceClassUnitPoint) {
    using T = TypeParam;
    constexpr T TOL = static_cast<T>(Eps<T>::VALUE * 5);

    EXPECT_NEAR((arms::maths::log_3<T, LN_PTR<T>>(static_cast<T>(1.0))), static_cast<T>(0.0), TOL);
    EXPECT_NEAR((arms::maths::log_5<T, LN_PTR<T>>(static_cast<T>(1.0))), static_cast<T>(0.0), TOL);
}

TYPED_TEST(LogTest, EquivalenceClassPositiveGreaterThanOne) {
    using T = TypeParam;
    constexpr XYZ<T> CASES[] = {
        {static_cast<T>(1.2), static_cast<T>(0.16595623285353023), static_cast<T>(0.11328275255937834)},
        {static_cast<T>(2.0), static_cast<T>(0.6309297535714574), static_cast<T>(0.43067655807339306)},
        {static_cast<T>(10.0), static_cast<T>(2.095903274289385), static_cast<T>(1.4306765580733933)},
        {static_cast<T>(42.0), static_cast<T>(3.4021735027328797), static_cast<T>(2.322344707681546)},
    };
    constexpr T TOL = static_cast<T>(Eps<T>::VALUE * 5);

    for (const auto &c : CASES) {
        const T log3_actual = arms::maths::log_3<T, LN_PTR<T>>(c.x);
        const T log5_actual = arms::maths::log_5<T, LN_PTR<T>>(c.x);

        EXPECT_GT(log3_actual, static_cast<T>(0.0));
        EXPECT_GT(log5_actual, static_cast<T>(0.0));
        EXPECT_NEAR(log3_actual, c.log3, TOL);
        EXPECT_NEAR(log5_actual, c.log5, TOL);
    }
}

TYPED_TEST(LogTest, BoundaryCharacteristicPoints) {
    using T = TypeParam;
    constexpr T TOL = static_cast<T>(Eps<T>::VALUE * 5);

    EXPECT_NEAR((arms::maths::log_3<T, LN_PTR<T>>(static_cast<T>(3.0))), static_cast<T>(1.0), TOL);
    EXPECT_NEAR((arms::maths::log_3<T, LN_PTR<T>>(static_cast<T>(9.0))), static_cast<T>(2.0), TOL);

    EXPECT_NEAR((arms::maths::log_5<T, LN_PTR<T>>(static_cast<T>(5.0))), static_cast<T>(1.0), TOL);
    EXPECT_NEAR((arms::maths::log_5<T, LN_PTR<T>>(static_cast<T>(25.0))), static_cast<T>(2.0), TOL);
}

TYPED_TEST(LogTest, BoundaryZeroAndNegativeDomain) {
    using T = TypeParam;

    EXPECT_TRUE((std::isinf(arms::maths::log_3<T, LN_PTR<T>>(static_cast<T>(0.0)))));
    EXPECT_TRUE((std::isinf(arms::maths::log_5<T, LN_PTR<T>>(static_cast<T>(0.0)))));
    EXPECT_LT((arms::maths::log_3<T, LN_PTR<T>>(static_cast<T>(0.0))), static_cast<T>(0.0));
    EXPECT_LT((arms::maths::log_5<T, LN_PTR<T>>(static_cast<T>(0.0))), static_cast<T>(0.0));

    EXPECT_TRUE((std::isnan(arms::maths::log_3<T, LN_PTR<T>>(static_cast<T>(-1.0)))));
    EXPECT_TRUE((std::isnan(arms::maths::log_5<T, LN_PTR<T>>(static_cast<T>(-1.0)))));
}

TYPED_TEST(LogTest, BoundaryNanAndInfinity) {
    using T = TypeParam;
    constexpr auto INF = std::numeric_limits<T>::infinity();
    constexpr auto NaN = std::numeric_limits<T>::quiet_NaN();

    EXPECT_TRUE((std::isnan(arms::maths::log_3<T, LN_PTR<T>>(NaN))));
    EXPECT_TRUE((std::isnan(arms::maths::log_5<T, LN_PTR<T>>(NaN))));

    EXPECT_TRUE((std::isinf(arms::maths::log_3<T, LN_PTR<T>>(INF))));
    EXPECT_TRUE((std::isinf(arms::maths::log_5<T, LN_PTR<T>>(INF))));
    EXPECT_GT((arms::maths::log_3<T, LN_PTR<T>>(INF)), static_cast<T>(0.0));
    EXPECT_GT((arms::maths::log_5<T, LN_PTR<T>>(INF)), static_cast<T>(0.0));
}

TEST(LogTestIntegral, EquivalenceClassIntegralOverload) {
    EXPECT_NEAR((arms::maths::log_3<int, LN_FLOAT_PTR>(1)), 0.0F, Eps<float>::VALUE * 5);
    EXPECT_NEAR((arms::maths::log_3<int, LN_FLOAT_PTR>(3)), 1.0F, Eps<float>::VALUE * 5);
    EXPECT_NEAR((arms::maths::log_3<int, LN_FLOAT_PTR>(9)), 2.0F, Eps<float>::VALUE * 5);

    EXPECT_NEAR((arms::maths::log_5<int, LN_FLOAT_PTR>(1)), 0.0F, Eps<float>::VALUE * 5);
    EXPECT_NEAR((arms::maths::log_5<int, LN_FLOAT_PTR>(5)), 1.0F, Eps<float>::VALUE * 5);
    EXPECT_NEAR((arms::maths::log_5<int, LN_FLOAT_PTR>(25)), 2.0F, Eps<float>::VALUE * 5);
}
