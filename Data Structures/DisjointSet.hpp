/*
  The definition of of this data structure can be found here:
  https://en.wikipedia.org/wiki/Disjoint-set_data_structure

  The identifier of a set (its key) in this implementation is a positive
  integer. If different type of key is needed it can easily be done with
  mapping from the key needed to the identifers this data structure
  provides. An example can be seen in "Greedy Algorithms/MSTGraph.hpp".
*/

#ifndef __DISJOINT_SET_HPP__
#define __DISJOINT_SET_HPP__

#include <vector>

class DisjointSet {
 public:
  // Construct an empty disjoint set.
  explicit DisjointSet() {}

  // Construct a new Disjoint set with subsets identifiers from 0 to n-1.
  explicit DisjointSet(std::size_t n);

  // Adds a new subset to the set.
  //
  // Returens the identifier of the new subset.
  int addSubset();

  // Returns the identifier of the subset 'set'. If 'set' is not a subset,
  // returns a negative identifier and the structure remains unchanged.
  int find(int set);

  // Union two subsets into one subset, i.e. both subsets will have the
  // same identifier.
  //
  // Returns the new identifier. If 'set1' or 'set2' is not a subset, returns
  // a negative identifier and the structure remains unchanged.
  int Union(int set1, int set2);

 private:
  std::vector<int> parent;
  std::vector<int> size;
};

#endif // !__DISJOINT_SET_HPP__
