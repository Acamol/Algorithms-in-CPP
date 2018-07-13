// Google Test headers
#include <gtest/gtest.h>

#include "../Dynamic Programming/KnapsackProblem.hpp"

namespace {

TEST(Knapsack, BasicCorrectness) {
  Knapsack a(7);
  std::vector<Knapsack::Box> vec;
  vec = { {5,3}, {6,10}, {1,1}, {2,5} }; // C++11
  a.chooseBoxes(vec);
  int profit = 0;
  for (auto& p : a.getBoxes()) {
    profit += p.first;
  }
  ASSERT_EQ(6, profit);
  profit = 0;
  vec[1].second = 4;
  a.clear();
  a.chooseBoxes(vec);
  for (auto& p : a.getBoxes()) {
    profit += p.first;
  }
  ASSERT_EQ(11, profit);
  profit = 0;

  Knapsack bigOne(50);
  vec = { {10,10}, {10,10}, {10, 10}, {10,10} }; // should take all of them
  bigOne.chooseBoxes(vec);
  for (auto& p : bigOne.getBoxes()) {
    profit += p.first;
  }
  ASSERT_EQ(40, profit);
  profit = 0;
  bigOne.clear();
  vec[3] = { 10, 30 };
  bigOne.chooseBoxes(vec);
  for (auto& p : bigOne.getBoxes()) {
    profit += p.first;
  }
  ASSERT_EQ(30, profit);
  profit = 0;
  vec[3] = { 10, 16 };
  vec.push_back({ 30, 5 });
  bigOne.clear();
  bigOne.chooseBoxes(vec);
  for (auto& p : bigOne.getBoxes()) {
    profit += p.first;
  }
  ASSERT_EQ(60, profit);
}

} // namespace