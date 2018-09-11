#ifndef __MERGE_SORT_HPP__
#define __MERGE_SORT_HPP__


#include <iterator>
#include <vector>

namespace Acamol {

template<class It>
using value_type_t = typename std::iterator_traits<It>::value_type;

// Merges two consecutive sorted ranges [first, middle) and [middle, last)
// into one sorted range [first, last). 
template<class InputIt, class Compare = std::less<value_type_t<InputIt>>>
void merge(InputIt begin, InputIt middle, InputIt end, Compare cmp = Compare{}) {
  std::vector<value_type_t<InputIt>> storage;
  storage.reserve(std::distance(begin, middle) + std::distance(middle, end));

  auto left_it = begin;
  auto right_it = middle;

  while (left_it != middle && right_it != end) {
    if (cmp(*left_it, *right_it)) {
      storage.push_back(*left_it);
      ++left_it;
    } else if (cmp(*right_it, *left_it)) {
      storage.push_back(*right_it);
      ++right_it;
    } else {
      storage.push_back(*left_it);
      ++left_it;
    }
  }

  while (left_it != middle) storage.push_back(*left_it), ++left_it;
  while (right_it != end) storage.push_back(*right_it), ++right_it;

  for (auto& v : storage) *begin = v, ++begin;
}

template<class InputIt, class Compare = std::less<value_type_t<InputIt>>>
void merge_sort(InputIt first, InputIt last, Compare cmp = Compare{}) {
  auto const n = std::distance(first, last);
  if (n < 2) return;

  auto const middle = std::next(first, n / 2);
  merge_sort(first, middle, cmp);
  merge_sort(middle, last, cmp);
  Acamol::merge(first, middle, last, cmp);
}

} // namespace

#endif // !__MERGE_SORT_HPP__
