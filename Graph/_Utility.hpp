#ifndef __UTILITY_GRAPH_HPP__
#define __UTILITY_GRAPH_HPP__

#include "../Data Structures/WeightedGraph.hpp"

#include <limits>

namespace Acamol { namespace internal {
  
template<class T>
void __initDistances(WeightedGraph<T>& graph, int s, std::unordered_map<int, double>& distances) {
  for (auto& pair : graph.getVertices()) {
    if (pair.second.getName() == s) {
      distances[s] = 0;
    } else {
      distances[pair.second.getName()] = std::numeric_limits<double>::max();
    }
  }
}

template<class T>
static std::list<const Vertex<T>*> __findSources(const Graph<T>& graph) {
  std::list<const Vertex<T>*> sources;
  for (auto& pair : graph.getVertices()) {
    const Vertex<T>& v = pair.second;
    if (v.getInDegree() == 0) {
      sources.push_back(&pair.second);
    }
  }
  return sources;
}


// Dijkstra uses this class as heap nodes
template <class T>
class HeapNode {
public:
  const Vertex<T>* v;
  double dist;

  HeapNode() {
  }

  HeapNode(const Vertex<T>& v, double initDist = std::numeric_limits<double>::max()) : v(&v), dist(initDist) {
  }

  friend bool operator<(const HeapNode& a, const HeapNode& b) {
    return a.dist < b.dist;
  }

  friend bool operator==(const HeapNode& a, const HeapNode& b) {
    return (a.dist == b.dist && a.v == b.v);
  }

  friend void swap(HeapNode& first, HeapNode& second) {
    using std::swap;

    swap(first.v, second.v);
    swap(first.dist, second.dist);
  }

};
  } }  // namespace

namespace std {
  // This hash function was chosen based on this discussion:
  // https://stackoverflow.com/questions/20953390/what-is-the-fastest-hash-function-for-pointers
  template <class T>
  struct hash<Acamol::internal::HeapNode<T>> {
    std::size_t operator()(const Acamol::internal::HeapNode<T>& e) const {
      static const size_t shift = (size_t)log2(1 + sizeof(e.v));
      return (size_t)(e.v) >> shift;
    }
  };
}

#endif // !__UTILITY_GRAPH_HPP__