#include "DisjointSet.hpp"

namespace Acamol { namespace DataStructures {

#define NO_PARENT -1
#define NO_SET -2

int DisjointSet::addSubset() {
  parent.push_back(NO_PARENT);
  size.push_back(1);
  return parent.size() - 1;
}

DisjointSet::DisjointSet(std::size_t n) {
  parent.resize(n, NO_PARENT);
  // all sets are singletons
  size.resize(n, 1);
}

int DisjointSet::find(int set) {
  if (set < 0 || set > parent.size() - 1) {
    return -1;
  }

  // find root - the identifier of the set
  int root = set;
  while (parent[root] != NO_PARENT) {
    root = parent[root];
  }

  // path compression - make all sets on the path from root to 'set' direct
  // sons of root
  while (set != root) {
    int currSet = set;
    set = parent[set];
    parent[currSet] = root;
  }

  return root;
}

int DisjointSet::Union(int set1, int set2) {
  set1 = find(set1);
  set2 = find(set2);

  if (set1 == NO_SET || set2 == NO_SET) {
    return NO_SET;
  }

  // union by size
  if (set1 != set2) {
    if (size[set1] < size[set2]) {
      parent[set1] = set2;
      ++size[set2];
      return set2;
    } else {
      parent[set2] = set1;
      ++size[set1];
      return set1;
    }
  }

  // 'set1' abd 'set2' are actually in the same subset
  return set1;
}

} }
