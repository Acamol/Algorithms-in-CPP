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

#include "../Data Structures/WeightedGraph.hpp"
#include "_Utility.hpp"

#include <unordered_map>

namespace Acamol {

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
template<class T>
std::pair<WeightedGraph<T>, std::unordered_map<int, double>> bellman_ford(const WeightedGraph<T>& graph, int source) {
  if (!graph.contains(source)) {
    return { WeightedGraph<T>(), std::unordered_map<int, double>() };
  }

  WeightedGraph<T> forest = graph;

  // since the graph uses Adjacency list, this map simplfy and reduces the 
  // time of finding an edge from one vertex to another.
  std::unordered_map<int, int> edgeFromToMap;

  // mapping from vertices to their distance (or total weight)
  std::unordered_map<int, double> vertexDist;

  // the graph we actually work on, and return, is a tree. so initially it
  // doesn't have any edges and distances between any two different vertices
  // is "infinity". once we preform a relaxation, we also add the edge.
  forest.clearEdges();

  // sets 's' distance to 0, and all other vertices to "infinity" if 's' is 
  // not a vertex in the graph, all distances should be "infinity", and the
  // graph is not connected.
  internal::__initDistances(forest, source, vertexDist);  

  // this is just a small optimization. when no more relaxations is possible
  // in a single iteration, the algorithm has effectivly finished its run.
  bool hasNotFinished = true;

  // iterate |V|-1 times over all edges
  for (std::size_t i = 1; i < graph.getNumOfVertices() && hasNotFinished; ++i) {
    hasNotFinished = false;
    for (auto& e : graph.getEdges()) {
      if (vertexDist[e.to] > vertexDist[e.from] + e.weight) {
        vertexDist[e.to] = vertexDist[e.from] + e.weight;

        // if the destination is not relaxed for the first time, then the previous edge
        // should be removed from 'forest'.
        if (edgeFromToMap.find(e.to) != edgeFromToMap.end()) {
          forest.removeEdge(e.to, edgeFromToMap[e.to]);
          edgeFromToMap.erase(e.to);
        }

        edgeFromToMap[e.to] = e.from;
        forest.addEdge(e.to, e.from, e.weight);
        hasNotFinished = true;
      }
    }
  }

  // if after |V|-1 times still relaxition is possible, it is promised 
  // we have a negative-weight cycle.
  for (auto& e : graph.getEdges()) {
    if (vertexDist[e.to] > vertexDist[e.from] + e.weight) {
      return { WeightedGraph<T>(), std::unordered_map<int, double>() };
    }
  }

  return { forest, vertexDist };
}

}

#endif // !__BELLMAN_FORD_HPP__
