#include <gtest/gtest.h>

#include <list>
#include <vector>

#include "merge_sort.hpp"

using namespace Acamol;

TEST(merge_sort, various_containers) {
  std::vector<int> vec{ 4, 3, 5, 2, 1 };
  merge_sort(vec.begin(), vec.end());
  ASSERT_TRUE(std::is_sorted(vec.begin(), vec.end()));

  std::list<int> list{ 4, 3, 5, 2, 1 };
  merge_sort(list.begin(), list.end());
  ASSERT_TRUE(std::is_sorted(list.begin(), list.end()));

  int array[] = { 4, 3, 5, 2, 1 };
  merge_sort(std::begin(array), std::end(array));
  ASSERT_TRUE(std::is_sorted(std::begin(array), std::end(array)));
}