#include <gtest/gtest.h>

#include <functional>
#include <vector>

#include <sorting/sorting.hpp>

TEST(BubbleSortTest, SortsAscendingByDefault) {
    std::vector<int> values{5, 1, 4, 2, 8};
    arms::sorting::bubble_sort(values.begin(), values.end());

    EXPECT_EQ(values, (std::vector<int>{1, 2, 4, 5, 8}));
}

TEST(BubbleSortTest, SortsDescendingWithComparator) {
    std::vector<int> values{5, 1, 4, 2, 8};
    arms::sorting::bubble_sort(values.begin(), values.end(), std::greater<>{});

    EXPECT_EQ(values, (std::vector<int>{8, 5, 4, 2, 1}));
}

TEST(BubbleSortTest, HandlesDuplicatesAndNegatives) {
    std::vector<int> values{3, -1, 3, 2, -1, 0};
    arms::sorting::bubble_sort(values.begin(), values.end());

    EXPECT_EQ(values, (std::vector<int>{-1, -1, 0, 2, 3, 3}));
}

TEST(BubbleSortTest, HandlesEmptyRange) {
    std::vector<int> values;
    arms::sorting::bubble_sort(values.begin(), values.end());

    EXPECT_TRUE(values.empty());
}

TEST(BubbleSortTest, HandlesSingleElement) {
    std::vector<int> values{42};
    arms::sorting::bubble_sort(values.begin(), values.end());

    EXPECT_EQ(values, (std::vector<int>{42}));
}
