#include <gtest/gtest.h>
#include <cmath>
#include <maths/maths.hpp>

constexpr long double sec(long double x) {
    return 1 / std::cos(x);
}

TEST(CosTest, ZeroInput) {
    EXPECT_EQ(1.0L, arms::maths::cos(0.0L, 1));
    EXPECT_EQ(1.0L, arms::maths::cos(0.0L, 10));
}

TEST(CosTest, SmallPositiveX) {
    long double x = 0.5L;
    long double expected = std::cos(x);

    EXPECT_NEAR(expected, arms::maths::cos(x, 3), 1e-3L);
    EXPECT_NEAR(expected, arms::maths::cos(x, 10), 1e-10L);
}

TEST(CosTest, PiOverTwo) {
    long double x = std::numbers::pi * 2;
    long double expected = std::cos(x);

    EXPECT_NEAR(expected, arms::maths::cos(x, 10), 1e-2L);
    EXPECT_NEAR(expected, arms::maths::cos(x, 20), 1e-10L);
}

TEST(CosTest, NegativeX) {
    long double x = -1.0L;
    long double expected = std::cos(x);
    EXPECT_NEAR(expected, arms::maths::cos(x, 15), 1e-10L);
}

TEST(CosTest, LargeTermsForConvergence) {
    long double x = 1.0L;
    long double expected = std::cos(x);
    EXPECT_NEAR(expected, arms::maths::cos(x, 50), 1e-15L);
}

TEST(CosTest, MinimumTerms) {
    EXPECT_EQ(1.0L, arms::maths::cos(3.0L, 1));
    EXPECT_EQ(1.0L, arms::maths::cos(3.0L, 0));
}

TEST(CosTest, Divergence) {
    long double x = 10.0L;
    long double expected = std::cos(x);

    EXPECT_GT(std::abs(arms::maths::cos(x, 5) - expected), 1.0L);
    EXPECT_NO_THROW(arms::maths::cos(x, 100));
}



TEST(SecTest, ZeroInput) {
    EXPECT_EQ(1.0L, arms::maths::sec(0.0L, 1));
    EXPECT_EQ(1.0L, arms::maths::sec(0.0L, 10));
}

TEST(SecTest, SmallPositiveX) {
    long double x = 0.5L;
    long double expected = sec(x);

    EXPECT_NEAR(expected, arms::maths::sec(x, 3), 1e-3L);
    EXPECT_NEAR(expected, arms::maths::sec(x, 10), 1e-10L);
}

TEST(SecTest, PiOverTwo) {
    long double x = std::numbers::pi * 2;
    long double expected = sec(x);

    EXPECT_NEAR(expected, arms::maths::sec(x, 10), 1e-2L);
    EXPECT_NEAR(expected, arms::maths::sec(x, 20), 1e-10L);
}

TEST(SecTest, NegativeX) {
    long double x = -1.0L;
    long double expected = sec(x);
    EXPECT_NEAR(expected, arms::maths::sec(x, 15), 1e-10L);
}

TEST(SecTest, LargeTermsForConvergence) {
    long double x = 1.0L;
    long double expected = sec(x);
    EXPECT_NEAR(expected, arms::maths::sec(x, 50), 1e-15L);
}

TEST(SecTest, MinimumTerms) {
    EXPECT_EQ(1.0L, arms::maths::sec(3.0L, 1));
    EXPECT_EQ(1.0L, arms::maths::sec(3.0L, 0));
}

TEST(SecTest, Divergence) {
    long double x = 10.0L;
    long double expected = sec(x);

    EXPECT_GT(std::abs(arms::maths::sec(x, 5) - expected), 1.0L);
    EXPECT_NO_THROW(arms::maths::sec(x, 100));
}
