#ifndef __INSERTION_SORT_HPP__
#define __INSERTION_SORT_HPP__

#include <iterator>
#include <functional>
#include <vector>

namespace Acamol {

template<class It>
using value_type_t = typename std::iterator_traits<It>::value_type;

template<class It, class Compare = std::less<value_type_t<It>>>
void insertion_sort(It first, It last, Compare cmp = Compare{}) {
  using std::swap;
  std::vector<value_type_t<It>> storage(first, last);

  for (std::size_t i = 1; i < storage.size(); ++i) {
    for (std::size_t j = i; j > 0; --j) {
      if (cmp(storage[j], storage[j - 1])) {
        swap(storage[j], storage[j - 1]);
      } else {
        break;
      }
    }
  }

  for (auto& v : storage) *first = v, ++first;
}

} // namespace

#endif // !__SELECTION_SORT_HPP__