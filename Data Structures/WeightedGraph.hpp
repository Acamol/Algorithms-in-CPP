#ifndef __WEIGHTEDGRAPH_HPP__
#define __WEIGHTEDGRAPH_HPP__

#include "../Data Structures/Graph.hpp"

#include <unordered_set>
#include <unordered_map>
#include <functional> // hash specialization

// TODO: implement move constructors

namespace Acamol { namespace DataStructures {

class Edge {
public:
  int from;
  int to;
  double weight;

  explicit Edge() {}

  explicit Edge(int from, int to, double weight = 0) : from(from), to(to), weight(weight) {
  }

  friend bool operator==(const Edge& e1, const Edge& e2) {
    return e1.from == e2.from && e1.to == e2.to;
  }

  friend bool operator<(const Edge& e1, const Edge& e2) {
    return e1.weight > e2.weight;
  }
};

} }
// injecting hash specialization to std to enable hasing edges and heap nodes.
namespace std {
  template<>
  struct hash<Acamol::DataStructures::Edge> {
    std::size_t operator()(const Acamol::DataStructures::Edge& e) const {
      std::size_t h1 = std::hash<int>()(e.from);
      std::size_t h2 = std::hash<int>()(e.to);

      return ((h1 ^ (h2 << 1))); // combine
    }
  };
}

namespace Acamol { namespace DataStructures {

template<class T>
class WeightedGraph : public Graph<T> {
 public:
  bool addEdge(int from, int to, double weight) {
    bool result = Graph<T>::addEdge(from, to);
    if (result) {
      edges.emplace(from, to, weight);
    }

    return result;
  }

  bool removeEdge(int from, int to) {
    bool result = Graph<T>::removeEdge(from, to);
    if (result) {
      edges.erase(Edge(from, to));
    }

    return result;
  }

  void clearEdges() {
    Graph<T>::clearEdges();
    edges.clear();
  }

  double getWeight(int from, int to) const {
    if (edges.find(Edge(from, to)) != edges.end()) {
      return edges.find(Edge(from, to))->weight;
    }

    throw std::out_of_range("WeightedGraph: edge from vertex " 
      + std::to_string(from) + " to vertex " + std::to_string(to) 
      + "does not exists.\n"); // C++11
  }

  double getTotalWeight() const {
    double weight = 0;
    for (auto& e : edges) {
      weight += e.weight;
    }

    return weight;
  }

  bool setWeight(int from, int to, double weight) {
    if (edges.find(Edge(from, to) != edges.end())) {
      edges.find(Edge(from, to))->weight = weight;
      return true;
    }

    return false;
  }

  const std::unordered_set<Edge>& getEdges() const {
    return edges;
  }

 private:
  std::unordered_set<Edge> edges;
};

} } // namespace

#endif // !__WEIGHTEDGRAPH_HPP__
