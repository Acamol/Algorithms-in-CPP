#include <algorithm>
#include "KnapsackProblem.hpp"

Knapsack::Knapsack(std::size_t maxWeight)
  : maxWeight(maxWeight), weight(0), boxes() {
}

std::vector<Knapsack::Box> Knapsack::getBoxes() const {
  return boxes;
}

void Knapsack::clear() {
  boxes.clear();
  weight = 0;
}

std::size_t Knapsack::size() const {
  return boxes.size();
}

void Knapsack::chooseBoxes(const std::vector<Box>& set) {
  if (maxWeight == weight) return;

  std::vector<std::vector<unsigned int>> A;
  // make room for a (number of boxes in the set)x(max weight) grid
  A.resize(set.size() + 1);
  for (auto& L : A) {
    L.resize(maxWeight + 1);
  }

  // calculate the grid, using dynamic programming
  // if we were not to calculate the actual boxes that we put inside the
  // knapsack, it would be sufficient to store only two lines at a time
  // which would reduce the space complexity from O(num_boxes*maxWeight)
  // to O(1).
  for (std::size_t i = 0; i <= set.size(); ++i) {
    for (std::size_t w = 0; w <= maxWeight; ++w) {
      if (i == 0 || w == 0) {
        A[i][w] = 0;
      } else if (set[i - 1].second > w) {
        A[i][w] = A[i - 1][w];
      } else if (set[i - 1].second <= w) {
        A[i][w] = std::max(A[i - 1][w], set[i - 1].first + A[i - 1][w - set[i - 1].second]);
      }
    }
  }

  // this part figures out the actual boxes we put inside the knapsack. we
  // reverse-engineer which cell in the grid "helped" A[i][w] to get its
  // value. if A[i][w] was calculated using A[i - 1][w], then the box set[i - 1]
  // is not in the optimal solution. if it was calculated using
  // A[i - 1][w - weight of box (i - 1)] then it is in the optimal solution.
  std::size_t i = set.size(), w = maxWeight;
  while (i > 0 && w > 0) {
    // check if the i-th box is in the optimal solution
    if (w >= set[i - 1].second && A[i][w] == A[i - 1][w - set[i - 1].second] + set[i - 1].first) {
      w -= set[i - 1].second;
      boxes.push_back(set[i - 1]);
      weight += set[i - 1].second;
    }
    --i;
  }
}