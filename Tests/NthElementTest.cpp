#include <gtest/gtest.h>

#include "nth_element.hpp"
#include "heap_sort.hpp"

#include <vector>
#include <random>
#include <limits>
#include <algorithm>

const std::size_t TEST_SIZE = 1000;

// generate a random vector, uniformly distributated from 0 to TEST_SIZE
static std::vector<int> generate_data(size_t size) {
  using value_type = int;
  // We use static in order to instantiate the random engine
  // and the distribution once only.
  // It may provoke some thread-safety issues.
  static std::uniform_int_distribution<value_type> distribution(0, TEST_SIZE);
  static std::default_random_engine generator;

  std::vector<value_type> data(size);
  std::generate(data.begin(), data.end(), []() { return distribution(generator); });
  return data;
}

TEST(nth_element_test, various) {
  std::vector<int> vec = generate_data(TEST_SIZE);
  ASSERT_THROW(Acamol::nth_element(vec.begin(), vec.end(), 0), std::out_of_range);
  ASSERT_THROW(Acamol::nth_element(vec.begin(), vec.end(), vec.size() + 1), std::out_of_range);

  auto vec_copy(vec);
  Acamol::heap_sort(vec_copy.begin(), vec_copy.end());
  for (std::size_t i = 1; i < vec.size(); ++i) {
    auto j = Acamol::nth_element(vec.begin(), vec.end(), i);
    ASSERT_EQ(j, vec_copy[i - 1]);
  }
}
