/*
  More information about shortest path problem can be found here:
  https://en.wikipedia.org/wiki/Shortest_path_problem

  This is an implementation of Dijkstra's algorithm, namely, solves
  the single-source shortest path problem with non-negative weights.
*/

#ifndef __DIJKSTRA_GRAPH_HPP__
#define __DIJKSTRA_GRAPH_HPP__

#include "_Utility.hpp"
#include "../Data Structures/Heap.hpp"
#include "../Data Structures/WeightedGraph.hpp"

#include <unordered_set>
#include <unordered_map>
#include <functional>
#include <algorithm>
#include <limits>


namespace Acamol { namespace Graph {

template<class T>
std::pair<Acamol::DataStructures::WeightedGraph<T>, std::unordered_map<int, double>> dijkstra(const Acamol::DataStructures::WeightedGraph<T>& graph, int source) {
  if (!graph.contains(source)) {
    return { Acamol::DataStructures::WeightedGraph<T>(), std::unordered_map<int, double>() };  // C++11
  }

  // the result graph
  Acamol::DataStructures::WeightedGraph<T> forest = graph; // copy the graph

  // Heap of vertices, sorted by distances from 's'.
  Acamol::DataStructures::Heap<internal::HeapNode<T>> Q;

  // mapping from vertices to their distance (or total weight)
  std::unordered_map<int, double> vertexDist;

  // marking of visited vertices
  std::unordered_set<int> visited;

  Q.push(internal::HeapNode<T>(graph.getVertex(source), 0));

  // sets 's' distance to 0, and all other vertices to "infinity" if 's' is 
  // not a vertex in the graph, all distances should be "infinity", and the
  // graph is not connected.
  internal::__initDistances(forest, source, vertexDist);  

  // mapping of edges inserted to 'forest'. used to reduce time to find an edge,
  // since WeightedGraph does not supply a way to get an edge.
  std::unordered_map<int, int> edgeFromToMap;

  // the graph we actually work on, and return, is a tree. so initially it
  // doesn't have any edges and distances between any two different vertices
  // is "infinity". once we preform a relaxation, we also add the edge.
  forest.clearEdges();

  while (!Q.empty()) {
    internal::HeapNode<T> top = Q.top();
    Q.pop();
    int from = top.v->getName();

    // if we visited this node, it means we already have a shorter path to it,
    // so we can just ignore any future occurrences of this node.
    if (visited.find(from) != visited.end()) {
      continue;
    }
    visited.insert(from);

    for (auto& pair : top.v->getNeighbours()) {
      Acamol::DataStructures::Vertex<T> * u = pair.second;
      int to = u->getName();
      double weight = graph.getWeight(from, to);
      double alt = top.dist + weight;

      // check if relaxation is needed
      if (alt < vertexDist[to]) {

        // if 'u' is not relaxed for the first time, then the previous edge
        // should be removed from 'forest'.
        if (edgeFromToMap.find(to) != edgeFromToMap.end()) {
          forest.removeEdge(to, edgeFromToMap[to]);
          edgeFromToMap.erase(to);
        }
        edgeFromToMap[to] = from;

        vertexDist[to] = alt;
        forest.addEdge(to, from, weight);

        // if the node we've just relaxed is discoverd for the first time
        // we need to push it to the heap.
        if (visited.find(to) == visited.end()) {
          Q.push(internal::HeapNode<T>(*u, alt));
        }
      }
    }
  }

  return { forest, vertexDist };  // C++11
}

} }


#endif // !__SHORTESTPATHGRAPH_HPP__
