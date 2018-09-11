#ifndef __QUICK_SORT_HPP__
#define __QUICK_SORT_HPP__

#include <iterator>
#include <vector>
#include <algorithm>
#include <random>

#include "insertion_sort.hpp"

namespace Acamol {

template<class It>
using value_type_t = typename std::iterator_traits<It>::value_type;

namespace internal {
  template<class InputIt, class Compare = std::less<value_type_t<InputIt>>>
  value_type_t<InputIt> __median_of_three(InputIt first, InputIt last, Compare cmp = Compare{}) {
    const std::size_t n = std::distance(first, last) - 1;
    std::default_random_engine g;
    std::uniform_int_distribution<int> dist(0, n);
    std::vector<value_type_t<InputIt>> storage;
    storage.reserve(3);

    storage.push_back(*std::next(first, dist(g)));
    storage.push_back(*std::next(first, dist(g)));
    storage.push_back(*std::next(first, dist(g)));
    insertion_sort(storage.begin(), storage.end(), cmp);

    return storage[1];
  }
}

template<class InputIt, class Compare = std::less<value_type_t<InputIt>>>
void quick_sort(InputIt first, InputIt last, Compare cmp = Compare{}) {
  const std::size_t n = std::distance(first, last);
  if (n <= 1) return;

  auto const pivot = internal::__median_of_three(first, last, cmp);
  auto const middle_l = std::partition(first, last, [=](const auto& e) { return cmp(e, pivot); });
  auto const middle_r = std::partition(middle_l, last, [=](const auto& e) { return !cmp(pivot, e); });
  quick_sort(first, middle_l, cmp);
  quick_sort(middle_r, last, cmp);
}

}  // namspace

#endif  // !__QUICK_SORT_HPP__