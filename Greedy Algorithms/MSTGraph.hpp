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

#include "../Data Structures/Graph.hpp"
#include "../Data Structures/DisjointSet.hpp"
#include <vector>
#include <algorithm>
#include <memory>

template<class T>
class MSTGraph : public Graph<T> {
public:
  typedef std::unique_ptr<MSTGraph<T>> MSTPtr;

  explicit MSTGraph() : Graph<T>(), edges(), weight(0) {
  }

  void addEdge(int from, int to, int weight) {
    if (this->contains(from) && this->contains(to)) {
      // both vertices exists
      Vertex<T>& v1 = vertices[from];
      Vertex<T>& v2 = vertices[to];
      v1.addNeighbour(&v2);
      v2.addNeighbour(&v1);
      ++numEdges;
      this->weight += weight;
      edges.push_back(Edge(from, to, weight));
    }
  }

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
  MSTPtr getMST() const {
    MSTPtr tree = std::make_unique<MSTGraph<T>>();

    // Creates |V| disjoint sets, with identifiers from 0 to |V|-1.
    DisjointSet sets(this->getNumOfVertices());
    // map from vertex identifier to set identifier
    std::unordered_map<int, int> vertexToSet;
    vertexToSet.reserve(this->getNumOfVertices());
    auto it = this->vertices.begin();
    // Defines the mapping between vertices to set identifiers
    for (std::size_t i = 0; i < this->getNumOfVertices(); ++i, ++it) {
      vertexToSet[it->second.getName()] = i;
    }

    std::vector<Edge> edgesCopy = edges;
    std::sort(edgesCopy.begin(), edgesCopy.end());

    // insert all vertices to the future tree. no edges at the moment.
    tree->vertices.reserve(this->getNumOfVertices());
    for (auto& pair : this->vertices) {
      tree->vertices[pair.first] = Vertex<T>(pair.second.getName(), pair.second.getData());
    }

    // it is not a tree as long as |E| < |V| - 1
    while (tree->getNumOfEdges() < tree->getNumOfVertices() - 1 && !edgesCopy.empty()) {
      Edge& e = edgesCopy.back();
      int setFrom = sets.find(vertexToSet[e.from]);
      int setTo = sets.find(vertexToSet[e.to]);

      // make sure the edge does not creates a circle.
      // if it does, it is not added to the future tree.
      if (setFrom != setTo) {
        sets.Union(setFrom, setTo);
        tree->addEdge(e.from, e.to, e.weight);
      }

      // pop the recent edge.
      edgesCopy.pop_back();
    }

    // if it is not a tree, reset 'tree' it to an empty graph.
    if (tree->getNumOfEdges() < tree->getNumOfVertices() - 1) {
      tree = std::make_unique<MSTGraph<T>>();
    }

    return tree;
  }

  int getWeight() const {
    return weight;
  }

private:
  class Edge {
  public:
    explicit Edge(int from, int to, int weight)
      : from(from), to(to), weight(weight) {
    }

    int from;
    int to;
    int weight;

    friend bool operator<(const MSTGraph<T>::Edge& e1, const MSTGraph<T>::Edge& e2) {
      return e1.weight > e2.weight;
    }
  };

  std::vector<Edge> edges;
  int weight;
};