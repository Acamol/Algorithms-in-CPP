/*
  The definition of Minimum Spanning Tree can be found here:
  https://en.wikipedia.org/wiki/Minimum_spanning_tree

  This implementation is based on Kruskal's algorithm. It basically works
  as follows:
  1. Given a graph G, create a new graph G' with the vertices of G and without edges.
  2. Create a set E of all edges in G.
  3. As long as G' is not connected or E is empty:
    3.1 Remove the lightest edge, e, from E.
    3.2 If adding e to G' does not create a circle, add e to G'.
  4. Return G'.

  Notice that if G' is connected then |E|=|V|-1, where |E| is the number of
  edges in G and |V| is the number of vertices in G. So if G' does not comply
  to that, then G does not have a spanning tree.

  Proof of correctness can be found on link above as well.

  Time complexity: O(|E|log|E|)=O(|E|log|V|).
*/

#ifndef __MIN_SPANNING_TREE_HPP__
#define __MIN_SPANNING_TREE_HPP__


#include "../Data Structures/WeightedGraph.hpp"
#include "../Data Structures/DisjointSet.hpp"

#include <vector>
#include <algorithm>

namespace Acamol {

// The actual algorithm implementation.
//
// Returns a unique_ptr to a new MSTGraph which is an MST of the graph, if
// one exists. An empty graph otherwise.
//
// BigO analysis:
// 1. Creating a disjoint set takes O(|V|).
// 2. Creating a mapping between vertices to subsets takes O(|V|).
// 3. Creating a new graph with no edges takes O(|V|).
// 4. Creating a copy of all edges takes O(|E|).
// 5. Sorting the edges takes O(|E|log|E|). 
// 6. find and union operation takes O(log|V|), adding an edge takes O(1).
//    The number of iterations is O(|E|), thus the loop takes
//    O(|E|log|V|+|E|log|V|).
//    
// Since O(|E|)=O(|V|^2), overall we get O(|E|log|E|).
template<class T>
WeightedGraph<T> min_spanning_tree(const WeightedGraph<T>& graph) {
  WeightedGraph<T> tree;

  // Creates |V| disjoint sets, with identifiers from 0 to |V|-1.
  DisjointSet sets(graph.getNumOfVertices());

  // map from vertex identifier to set identifier
  std::unordered_map<int, int> vertexToSet;
  vertexToSet.reserve(graph.getNumOfVertices());  // optimization
  auto it = graph.getVertices().begin();

  // Defines the mapping between vertices to set identifiers
  for (std::size_t i = 0; i < graph.getNumOfVertices(); ++i, ++it) {
    vertexToSet[it->second.getName()] = i;
  }

  std::vector<internal::Edge> edgesCopy(graph.getEdges().begin(), graph.getEdges().end());
  std::sort(edgesCopy.begin(), edgesCopy.end());

  // insert all vertices to the future tree. no edges at the moment.
  for (auto& pair : graph.getVertices()) {
    tree.addVertex(pair.first, pair.second.getData());
  }

  // it is not a tree as long as |E| < |V| - 1
  while (tree.getNumOfEdges() < tree.getNumOfVertices() - 1 && !edgesCopy.empty()) {
    internal::Edge& e = edgesCopy.back();
    int setFrom = sets.find(vertexToSet[e.from]);
    int setTo = sets.find(vertexToSet[e.to]);

    // make sure the edge does not create a circle.
    // if it does, it is not added to the future tree.
    if (setFrom != setTo) {
      sets.Union(setFrom, setTo);
      tree.addEdge(e.from, e.to, e.weight);
    }

    // pop the recent edge.
    edgesCopy.pop_back();
  }

  // if it is not a tree yet, reset 'tree' to an empty graph.
  if (tree.getNumOfEdges() < tree.getNumOfVertices() - 1) {
    tree.clear();
    tree.clearEdges();
  }

  return tree;
}

}  // namespace

#endif // !__MIN_SPANNING_TREE_HPP__
