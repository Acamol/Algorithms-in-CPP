/*
  This algorithm is based on "Median of medians". More information here:
  https://en.wikipedia.org/wiki/Median_of_medians

  The algorithm rearranges the elements in a given range in such a way that
  the element at the nth position is the element that would be in that
  position in a sorted sequence and returns the nth element.

  It works very similarly to Quicksort:
  1. If the input size is less than 10, sort it and return the nth element.
  2. Choose a pivot (more on this below).
  3. Divide the input such that all elements that are less than the pivot
     precede it, and all the elements that are greater or equal follow it.
     (So at this point, the pivot is in its position as if the input was sorted)
  4. If the pivot is in the nth position, we return the pivot.
  5. If the pivot position is less than n, go to step 1 with the input starting
     from the pivot and the following elements. Otherwise, go to step 1 with
     the input statring from the start up to the pivot.

  To archive deterministic time complexity the pivot is selected like so:
  - Divide the input into fifths.
  - Calculate the median of each fifth.
  - Choose the pivot as the median of the medians from the previous step.

  It can be shown that after a recursive call, the number of elements left
  is at most 7n/10 + 6. Thus, the function's recurrence relation T fulfills:
  T(n) <= T(ceil(n/5)) + T(7n/10+6) + O(n)
  By induction it easy to show that T(n) = O(n), thus the algorithm is O(n)
  time complexity.
*/

#ifndef __NTH_ELEMENT_HPP__
#define __NTH_ELEMENT_HPP__

#include <iterator>
#include <algorithm>
#include <vector>

#include "insertion_sort.hpp"

namespace Acamol {

template<class It>
using value_type_t = typename std::iterator_traits<It>::value_type;

namespace internal {

// divide the input into vector of vectors, each of size 5 (apart of, maybe, the last one).
template<class It>
std::vector<std::vector<value_type_t<It>>> __divide_range_to_fifths(It begin, It end) {
  const std::size_t n = std::distance(begin, end);
  std::vector<std::vector<value_type_t<It>>> fifths;
  fifths.resize(n / 5 + ((n % 5 == 0) ? 0 : 1));
  
  for (auto& v : fifths) {
    v.reserve(5);
  }

  auto it = begin;
  for (std::size_t i = 0, j = 0; i < n; ++i, ++it) {
    j = (i % 5 != 0) ? j : i / 5;
    fifths[j].push_back(*it);
  }

  return fifths;
}

// use insertion sort to sort each fifth and return a vector of medians
template<class It, class Compare = std::less<value_type_t<It>>>
std::vector<value_type_t<It>> __find_median_of_fifths(std::vector<std::vector<value_type_t<It>>>& vec, Compare cmp = Compare{}) {
  std::vector<value_type_t<It>> medians;
  medians.reserve(vec.size());

  std::size_t i = 0;
  for (auto& v : vec) {
    insertion_sort(v.begin(), v.end(), cmp);
    medians.push_back(v[v.size() / 2]);
  }

  return medians;
}

template<class It, class Compare>
value_type_t<It> __nth_element_work(It begin, It end, std::size_t n, Compare cmp) {
  if (std::distance(begin, end) < 10) {
    // if there are less than 10 elements,
    // just sort the and return the nth element
    insertion_sort(begin, end, cmp);
    return *std::next(begin, n - 1);
  }

  // divide the elements to vectors of five elements each
  auto fifths = internal::__divide_range_to_fifths(begin, end);

  // get a vector with the median of each vector described earlier
  auto medians = internal::__find_median_of_fifths<It, Compare>(fifths);
  fifths.clear();  // fifths is not needed anymore (space optimization)

  // calculate the median of medians with a recursive call
  value_type_t<It> median_of_medians = __nth_element_work(medians.begin(), medians.end(), medians.size() / 2 + 1, cmp);
  medians.clear();  // medians is not needed anymore

  // partition acording to 'median_of_medians'. i.e. all elements less than
  // 'median_of_medians' will left to it, and elements greater or equal will
  // right to it. effectivly, 'median_of_medians' will get its right place.
  auto pos = std::partition(begin, end, [=](const auto& e) { return cmp(e, median_of_medians); });
  // now 'pos' points to the median of medians, and it's in its right place

  auto dist = std::distance(begin, pos);
  if (dist + 1 == n) {
    // pos is the nth element
    return median_of_medians;
  }
  if (dist >= n) {
    // pos is greater than the nth element,
    // we can limit our search to the range [begin, pos)
    return __nth_element_work(begin, pos, n, cmp);
  } else {
    // pos is lower than the nth element,
    // we can limit our search to the range [pos, end)
    return __nth_element_work(pos, end, n - dist, cmp);
  }
}

}

template<class It, class Compare = std::less<value_type_t<It>>>
value_type_t<It> nth_element(It begin, It end, std::size_t n, Compare cmp = Compare{}) {
  auto k = std::distance(begin, end);
  if (n == 0 || std::distance(begin, end) < n) {
    throw std::out_of_range("error: nth_element:: n == " + std::to_string(n));
  }

  return internal::__nth_element_work(begin, end, n, cmp);
}

}  // namespace

#endif // !__NTH_ELEMENT_HPP__
