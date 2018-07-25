/*
  More information about shortest path problem can be found here:
  https://en.wikipedia.org/wiki/Shortest_path_problem

  And Bellman-Ford algorithm:
  https://en.wikipedia.org/wiki/Bellman–Ford_algorithm

  Bellman-Ford algorithm is essentially an example of dynamic programming.
  It can be shown that after the i-th "round" all shortest paths with i edges
  are correct. Like Dijkstra's algorithm, it uses "Relaxation", but also
  allows negative weights, and even can detect a negative-weight cycle.

  This implementation returns an empty graph once a negative-weight cycle has
  been detected, but with a small change it is possible to assign each vertex
  its correct distance, including the vertices which are arbitrary far.
*/

#ifndef __BELLMAN_FORD_HPP__
#define __BELLMAN_FORD_HPP__

#include "../Data Structures/Graph.hpp"
#include <unordered_map>
#include <limits>
#include <memory>
#include <functional>
#include <algorithm>

class Edge {
public:
  int from;
  int to;

  explicit Edge() {}

  explicit Edge(int from, int to) : from(from), to(to) {
  }

  friend bool operator==(const Edge& e1, const Edge& e2) {
    return e1.from == e2.from && e1.to == e2.to;
  }
};

namespace std {
  template<>
  struct hash<Edge> {
    std::size_t operator()(const Edge& e) const {
      std::size_t h1 = std::hash<int>()(e.from);
      std::size_t h2 = std::hash<int>()(e.to);

      return ((h1 ^ (h2 << 1))); // combine
    }
  };
}


template <class T>
class BellmanFordGraph : public Graph<T> {
public:
  typedef std::unique_ptr<BellmanFordGraph<T>> SPForestPtr;

  void addEdge(int from, int to, double weight) {
    Graph<T>::addEdge(from, to);
    // also maps edge to its weight. the graph allows parallel edges, but
    // edgeWeights map keeps only the minimal edge of parallel, as this is
    // enough to solve the SSSP problem.
    if (this->contains(from) && this->contains(to)) {
      if (edges.find(Edge(from, to)) != edges.end()) {
        edges[Edge(from, to)] = std::min(weight, edges[Edge(from, to)]);
      } else {
        edges[Edge(from, to)] = weight;
      }
    }
  }

  void removeEdge(int from, int to) {
    Graph<T>::removeEdge(from, to);
    // Uh-oh! if there are parallel edges, we don't know the weight of the one
    // that we don't remove. 
    edges.erase(Edge(from, to));
  }

  double getDistance(int name) const {
    if (!contains(name)) return -1;
    return vertexDist.find(name)->second;
  }

  // If the graph contains a negative-weight cycle, returns an empty graph.
  // If the graph is connected, returns a tree of paths from each vertex
  // to 's'. The result graph would store these distances as well
  // (use getDistance() to access).
  // If it is not connected, the result graph would be a forest. i.e. the
  // it would be a disjoint union of trees.
  // If 's', the vertex to start from, is not in the graph,
  // returns an empty graph.
  //
  // The result graph is returned as unique_ptr.
  //
  // 's' is the vertex to start from.
  //
  // TODO: add bigO analysis.
  SPForestPtr getShortestPaths(int s) {
    if (!contains(s)) {
      return std::make_unique<BellmanFordGraph<T>>();
    }

    SPForestPtr forest = std::make_unique<BellmanFordGraph<T>>(*this);

    // since the graph uses Adjacency list, this map simplfy and reduces the 
    // time of finding an edge from one vertex to another.
    std::unordered_map<int, int> edgeFromToMap;

    // the graph we actually work on, and return, is tree. so initially it
    // doesn't have any edges and distances between any two different vertices
    // is "infinity". once we preform a relaxation, we also add the edge.
    forest->clearEdges();
    forest->edges.clear();
    forest->setToNotRechable(s);

    // this is just a small optimization. when no more relaxations is possible
    // in a single iteration, the algorithm has effectivly finished its run.
    bool hasNotFinished = true;

    // iterate |V|-1 times over all edges
    for (std::size_t i = 1; i < getNumOfVertices() && hasNotFinished; ++i) {
      hasNotFinished = false;
      for (auto& e : edges) {
        if (forest->vertexDist[e.first.to] > forest->vertexDist[e.first.from] + e.second) {
          forest->vertexDist[e.first.to] = forest->vertexDist[e.first.from] + e.second;

          // if the destination is not relaxed for the first time, then the previous edge
          // should be removed from 'forest'.
          if (edgeFromToMap.find(e.first.to) != edgeFromToMap.end()) {
            forest->removeEdge(e.first.to, edgeFromToMap[e.first.to]);
            edgeFromToMap.erase(e.first.to);
          }

          edgeFromToMap[e.first.to] = e.first.from;
          forest->addEdge(e.first.to, e.first.from, e.second);
          hasNotFinished = true;
        }
      }
    }

    // if after |V|-1 times still relaxition is possible, it is promised 
    // we have a negative-weight cycle.
    for (auto& e : edges) {
      if (forest->vertexDist[e.first.to] > forest->vertexDist[e.first.from] + e.second) {
        return std::make_unique<BellmanFordGraph<T>>();
      }
    }

    return forest;
  }

private:
  std::unordered_map<Edge, double> edges;
  std::unordered_map<int, double> vertexDist;

  void setToNotRechable(int s) {
    for (auto& pair : this->vertices) {
      if (pair.second.getName() == s) {
        vertexDist[s] = 0;
      } else {
        vertexDist[pair.second.getName()] = std::numeric_limits<double>::max();
      }
    }
  }
};


#endif // !__BELLMAN_FORD_HPP__
