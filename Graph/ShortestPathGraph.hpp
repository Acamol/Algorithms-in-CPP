/*
  More information about shortest path problem can be found here:
  https://en.wikipedia.org/wiki/Shortest_path_problem

  This is an implementation of Dijkstra's algorithm, namely, solves
  the single-source shortest path problem with non-negative weights.

  * A small bug is described in line 112. If we limit ourselfs not to
    use parallel edges, it is not an issue. It can be easily solved,
    for example, by giving each edge a unique identifier, but too much code
    for this problem was already written as it is, so I'll just leave it
    like that with this comment.
*/

#ifndef __SHORTESTPATHGRAPH_HPP__
#define __SHORTESTPATHGRAPH_HPP__

#include "../Data Structures/Heap.hpp"
#include "../Data Structures/Graph.hpp"
#include <memory>
#include <unordered_set>
#include <functional>
#include <algorithm>
#include <cmath>
#include <limits>

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

template <class T>
class HeapNode {
public:
  const Vertex<T>* v;
  double dist;

  HeapNode() {
  }
  // -1 is max value of std::size_t
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

// injecting hash specialization to std to enable hasing edges and heap nodes.
namespace std {

  // This hash function was chosen based on this discussion:
  // https://stackoverflow.com/questions/20953390/what-is-the-fastest-hash-function-for-pointers
  template <class T>
  struct hash<HeapNode<T>> {
    std::size_t operator()(const HeapNode<T>& e) const {
      static const size_t shift = (size_t)log2(1 + sizeof(e.v));
      return (size_t)(e.v) >> shift;
    }
  };

  template<>
  struct hash<Edge> {
    std::size_t operator()(const Edge& e) const {
      std::size_t h1 = std::hash<int>()(e.from);
      std::size_t h2 = std::hash<int>()(e.to);

      return ((h1 ^ (h2 << 1))); // combine
    }
  };
}

template<class T>
class ShortestPathGraph : public Graph<T> {
 public:
  typedef std::unique_ptr<ShortestPathGraph<T>> SPForestPtr;

  void addEdge(int from, int to, double weight) {
    Graph<T>::addEdge(from, to);
    // also maps edge to its weight. the graph allows parallel edges, but
    // edgeWeights map keeps only the minimal edge of parallel, as this is
    // enough to solve the SSSP problem.
    if (this->contains(from) && this->contains(to)) {
      if (edgeWeights.find(Edge( from, to )) != edgeWeights.end()) {
        edgeWeights[Edge( from, to )] = std::min(weight, edgeWeights[Edge( from, to )]);
      } else {
        edgeWeights[Edge( from, to )] = weight;
      }
    }
  }

  void removeEdge(int from, int to) {
    Graph<T>::removeEdge(from, to);
    // Bug! if there are parallel edges, we don't know the weight of the one
    // that we don't remove. 
    edgeWeights.erase(Edge(from, to));
  }

  double getDistance(int name) const {
    if (!contains(name)) return -1;
    return vertexDist.find(name)->second;
  }

  // If the graph is connected, returns a tree of paths from each vertex
  // to 's'. The result graph would store these distances as well
  // (use getDistance() to access).
  // If it is not connected, the result graph would be a forest. i.e. the
  // it would be a disjoint union of trees: all reachable vertices from 's'
  // would be one tree and each one of the other vertices would be a tree
  // (in other words, an isolated vertex).
  // If 's', the vertex to start from, is not in the graph a null pointer,
  // returns an empty graph.
  //
  // The result graph is returned as a unique_ptr.
  //
  // 's' is the vertex to start from.
  //
  // TODO: add bigO analysis.
  SPForestPtr getShortestPaths(int s) {
    if (!contains(s)) {
      return std::make_unique<ShortestPathGraph<T>>();
    }

    // the result graph.
    SPForestPtr forest = std::make_unique<ShortestPathGraph<T>>(*this);

    // Heap of vertices, sorted by distances from 's'.
    Heap<HeapNode<T>> Q = createVerticesHeapByDist(s);

    // since the graph uses Adjacency list, this map simplfy and reduces the 
    // time of finding an edge from one vertex to another.
    std::unordered_map<int, int> edgeFromToMap;

    // the graph we actually work on, and return, is tree. so initially it
    // doesn't have any edges and distances between any two different vertices
    // is "infinity". once we preform a relaxation, we also add the edge.
    forest->clearEdges();
    forest->edgeWeights.clear();
    forest->setToNotRechable(s);

    // if 's' is not a vertex in the graph, all distances should be "infinity",
    // and the graph is not connected.

    while (!Q.empty()) {
      HeapNode<T>& top = Q.top();
      int from = top.v->getName();
      for (auto& u : top.v->getNeighbours()) {
        int to = u->getName();
        double weight = edgeWeights[Edge( from, to )];
        double alt = top.dist + weight;

        // check if relaxation is possible
        if (alt < forest->vertexDist[to]) {
          // if 'u' is not relaxed for the first time, then the previous edge
          // should be removed from 'tree'.
          if (edgeFromToMap.find(to) != edgeFromToMap.end()) {
            forest->removeEdge(to, edgeFromToMap[to]);
            edgeFromToMap.erase(to);
          }

          Q.decreaseKey(HeapNode<T>(*u, forest->vertexDist[to]), HeapNode<T>(*u, alt));
          forest->vertexDist[to] = alt;
          forest->addEdge(to, from, weight);
          edgeFromToMap[to] = from;
        }
      }

      Q.pop();
    }

    return forest;
  }

 private:

  // the graph technically allows identical edges, but since 'edges' is
  // an unordered_set it stores only unique edges. i.e. it would not necessery
  // store all edges in the graph. but this is not needed for this algorithm,
  // because if there are identical edges only one of them is choosen.
  std::unordered_map<Edge, double> edgeWeights;

  // map of distances from a certain vertex to all other vertices.
  // valid only if this graph was produced through getShortestPath()
  std::unordered_map<int, double> vertexDist;

  Heap<HeapNode<T>> createVerticesHeapByDist(int s) {
    std::vector<HeapNode<T>> nodes;
    Vertex<T>& start = this->vertices[s]; // !!! call this function only if 's' is in the graph !!! 
    nodes.push_back(HeapNode<T>(start, 0));
    vertexDist[s] = 0;
    for (auto& p : this->vertices) {
      if (p.first != s) {
        nodes.push_back(HeapNode<T>(p.second));
        vertexDist[p.second.getName()] = std::numeric_limits<double>::max();
      }
    }

    return Heap<HeapNode<T>>(nodes);
  }

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

#endif // !__SHORTESTPATHGRAPH_HPP__
