#pragma once

#include <algorithm>
#include <concepts>
#include <functional>
#include <iterator>

namespace arms::sorting {

template <std::random_access_iterator RandomIt, class Compare>
requires std::indirect_strict_weak_order<Compare, RandomIt>

constexpr void bubble_sort(RandomIt first, RandomIt last, Compare comp) {
    if (first == last) {
        return;
    }

    for (auto unsorted_end = last; unsorted_end != first; --unsorted_end) {
        bool swapped = false;
        for (auto it = first; (it + 1) < unsorted_end; ++it) {
            auto next = it + 1;
            if (std::invoke(comp, *next, *it)) {
                std::iter_swap(it, next);
                swapped = true;
            }
        }

        if (!swapped) {
            return;
        }
    }
}

template <std::random_access_iterator RandomIt>
constexpr void bubble_sort(RandomIt first, RandomIt last) {
    bubble_sort(first, last, std::less<>{});
}

}  // namespace arms::sorting
