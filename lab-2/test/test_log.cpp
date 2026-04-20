#include <gtest/gtest.h>
#include <cmath>
#include <cstddef>
#include <limits>
#include <type_traits>

#include <maths/defaults.hpp>
#include <maths/ln.hpp>
#include <maths/log.hpp>

using arms::maths::precision::Eps;

template <typename T>
class LogTest : public testing::Test {};

using FloatingPointTypes = testing::Types<float, double, long double>;
TYPED_TEST_SUITE(LogTest, FloatingPointTypes);

template <typename T>
inline constexpr auto LN_PTR = static_cast<T (*)(T, size_t)>(arms::maths::ln);

constexpr auto LN_FLOAT_PTR = static_cast<float (*)(float, size_t)>(arms::maths::ln);

TYPED_TEST(LogTest, TestDomainAndSpecialValues) {
    using T = TypeParam;
    constexpr auto INF = std::numeric_limits<T>::infinity();
    constexpr auto NaN = std::numeric_limits<T>::quiet_NaN();

    EXPECT_TRUE((std::isnan(arms::maths::log_3<T, LN_PTR<T>>(static_cast<T>(-1.0)))));
    EXPECT_TRUE((std::isnan(arms::maths::log_5<T, LN_PTR<T>>(static_cast<T>(-1.0)))));
    EXPECT_TRUE((std::isnan(arms::maths::log_3<T, LN_PTR<T>>(NaN))));
    EXPECT_TRUE((std::isnan(arms::maths::log_5<T, LN_PTR<T>>(NaN))));

    EXPECT_TRUE((std::isinf(arms::maths::log_3<T, LN_PTR<T>>(static_cast<T>(0.0)))));
    EXPECT_TRUE((std::isinf(arms::maths::log_5<T, LN_PTR<T>>(static_cast<T>(0.0)))));
    EXPECT_LT((arms::maths::log_3<T, LN_PTR<T>>(static_cast<T>(0.0))), static_cast<T>(0.0));
    EXPECT_LT((arms::maths::log_5<T, LN_PTR<T>>(static_cast<T>(0.0))), static_cast<T>(0.0));

    EXPECT_TRUE((std::isinf(arms::maths::log_3<T, LN_PTR<T>>(INF))));
    EXPECT_TRUE((std::isinf(arms::maths::log_5<T, LN_PTR<T>>(INF))));
    EXPECT_GT((arms::maths::log_3<T, LN_PTR<T>>(INF)), static_cast<T>(0.0));
    EXPECT_GT((arms::maths::log_5<T, LN_PTR<T>>(INF)), static_cast<T>(0.0));
}

TYPED_TEST(LogTest, TestCharacteristicPoints) {
    using T = TypeParam;
    constexpr T TOL = static_cast<T>(Eps<T>::VALUE * 5);

    EXPECT_NEAR((arms::maths::log_3<T, LN_PTR<T>>(static_cast<T>(1.0))), static_cast<T>(0.0), TOL);
    EXPECT_NEAR((arms::maths::log_3<T, LN_PTR<T>>(static_cast<T>(3.0))), static_cast<T>(1.0), TOL);
    EXPECT_NEAR((arms::maths::log_3<T, LN_PTR<T>>(static_cast<T>(9.0))), static_cast<T>(2.0), TOL);

    EXPECT_NEAR((arms::maths::log_5<T, LN_PTR<T>>(static_cast<T>(1.0))), static_cast<T>(0.0), TOL);
    EXPECT_NEAR((arms::maths::log_5<T, LN_PTR<T>>(static_cast<T>(5.0))), static_cast<T>(1.0), TOL);
    EXPECT_NEAR((arms::maths::log_5<T, LN_PTR<T>>(static_cast<T>(25.0))), static_cast<T>(2.0), TOL);
}

TYPED_TEST(LogTest, TestRangeAgainstStdLog) {
    using T = TypeParam;
    constexpr T VALUES[] = {
        static_cast<T>(0.1), static_cast<T>(0.25), static_cast<T>(0.5),
        static_cast<T>(0.75), static_cast<T>(1.2), static_cast<T>(2.0),
        static_cast<T>(10.0), static_cast<T>(42.0)
    };
    constexpr T TOL = static_cast<T>(Eps<T>::VALUE * 5);

    for (const T value : VALUES) {
        const T expected_log3 =
            static_cast<T>(std::log(static_cast<long double>(value)) / std::log(static_cast<long double>(3.0)));
        const T expected_log5 =
            static_cast<T>(std::log(static_cast<long double>(value)) / std::log(static_cast<long double>(5.0)));

        EXPECT_NEAR((arms::maths::log_3<T, LN_PTR<T>>(value)), expected_log3, TOL);
        EXPECT_NEAR((arms::maths::log_5<T, LN_PTR<T>>(value)), expected_log5, TOL);
    }
}

TEST(LogTestIntegral, TestIntegralInput) {
    EXPECT_NEAR((arms::maths::log_3<int, LN_FLOAT_PTR>(1)), 0.0f, Eps<float>::VALUE * 5);
    EXPECT_NEAR((arms::maths::log_3<int, LN_FLOAT_PTR>(3)), 1.0f, Eps<float>::VALUE * 5);
    EXPECT_NEAR((arms::maths::log_3<int, LN_FLOAT_PTR>(9)), 2.0f, Eps<float>::VALUE * 5);

    EXPECT_NEAR((arms::maths::log_5<int, LN_FLOAT_PTR>(1)), 0.0f, Eps<float>::VALUE * 5);
    EXPECT_NEAR((arms::maths::log_5<int, LN_FLOAT_PTR>(5)), 1.0f, Eps<float>::VALUE * 5);
    EXPECT_NEAR((arms::maths::log_5<int, LN_FLOAT_PTR>(25)), 2.0f, Eps<float>::VALUE * 5);
}
