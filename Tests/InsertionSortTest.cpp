#include <gtest/gtest.h>

#include "insertion_sort.hpp"

#include <vector>
#include <list>
#include <algorithm>

using namespace Acamol;

TEST(selection_sort_test, various_containers) {
  std::vector<int> vec{ 4, 3, 5, 2, 1 };
  insertion_sort(vec.begin(), vec.end());
  ASSERT_TRUE(std::is_sorted(vec.begin(), vec.end()));

  std::list<int> list{ 4, 3, 5, 2, 1 };
  insertion_sort(list.begin(), list.end());
  ASSERT_TRUE(std::is_sorted(list.begin(), list.end()));

  int array[] = { 4, 3, 5, 2, 1 };
  insertion_sort(std::begin(array), std::end(array));
  ASSERT_TRUE(std::is_sorted(std::begin(array), std::end(array)));
}