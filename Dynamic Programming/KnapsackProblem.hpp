/*
  The definition of the Knapsack Problem can be found here:
  https://en.wikipedia.org/wiki/Knapsack_problem

  This algorithm solves the 1-dimensional problem using Dynamic Programming.
  The actual algorithm is the method Knapsack::chooseBoxes.

  Time and space complexity: O(nW), when n is the number of boxes and W is
                             the knapsack's maximum weight.
*/

#ifndef __KNAPSACK_PROBLEM_HPP__
#define __KNAPSACK_PROBLEM_HPP__

#include <vector>

class Knapsack {
public:
  // first is value, second is weight
  typedef std::pair<unsigned int, unsigned int> Box;

  explicit Knapsack(std::size_t maxWeight);

  // Given a set of boxes, each with a weight and a value, determines the
  // boxes to include in a collection so that the total weight is less than
  // or equal to a given limit and the total value is as large as possible.
  void chooseBoxes(const std::vector<Box>& set);

  // Returns a copy of the boxes inside the knapsack.
  std::vector<Box> getBoxes() const;

  // Empties the knapsack.
  void clear();

  // Returns the number of boxes.
  std::size_t size() const;

private:
  const std::size_t maxWeight; // the max weight of boxes the knapsack can hold
  std::size_t weight; // the current total weight of boxes inside the knapsack
  std::vector<Box> boxes; // the boxes inside the knapsack
};

#endif // !__KNAPSACK_PROBLEM_HPP__
