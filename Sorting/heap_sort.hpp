#ifndef __HEAPSORT_HPP__
#define __HEAPSORT_HPP__

#include "../Data Structures/Heap.hpp"

#include <iterator>
#include <functional>

namespace Acamol { 

template<class It>
using value_type_t = typename std::iterator_traits<It>::value_type;

template<class RandomAccessIterator, class Compare = std::less<value_type_t<RandomAccessIterator>>>
void heap_sort(RandomAccessIterator first, RandomAccessIterator last, Compare cmp = Compare{}) {
  Heap<value_type_t<RandomAccessIterator>, Compare> heap(first, last);

  for (auto it = first; it != last; ++it) {
    *it = heap.top();
    heap.pop();
  }
}

}
#endif // !__HEAPSORT_HPP__
