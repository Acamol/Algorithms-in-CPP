/*
The definition of Depth-First Search can be found here:
https://en.wikipedia.org/wiki/Depth-first_search

The complexity is O(V+E).
*/

#ifndef __DFSGRAPH_HPP__
#define __DFSGRAPH_HPP__

#include "../Data Structures/Graph.hpp"
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <memory>

namespace Acamol { 

/*
    Returns a unique_ptr a Graph representation of a DFS tree whose root is
    the vertex 'name'. If no such vertex exists, returns an empty tree.

    Notice that the returned Graph is not actually a tree by the formal
    mathematical definition. Each vertex in the returned graph points to it
    parent (except for the root), as illustrated in the example below.

    Example:
      Graph<int> g;
      g.addVertex(0, 2);
      gaddVertex(1, 3);
      g.addVertex(2, 5);
      g.addEdge(0, 1);
      g.addEdge(0, 2);
      DFSTree tree = g.DFS(0);
      tree->getData(0);

    So a diagram of g can be thought of something like this:

          +-+
      +---+0+---+
      |   +-+   |
      |         |
     +v+       +v+
     |1|       |2|
     +-+       +-+

    And this is how (*tree) looks like:

          +-+
      +--->0<---+
      |   +-+   |
      |         |
     +++       +++
     |1|       |2|
     +-+       +-+

    Complexity: O(V+E) (theortically, can get worse if the map behaves really bad)
*/

template<class T>
Graph<T> dfs(const Graph<T>& graph, int source) {
  Graph<T> tree;
  if (!graph.contains(source)) return tree;

  const Vertex<int>& start = graph.getVertex(source);
  std::unordered_set<int> discovered;
  std::stack<const Vertex<T>*> S;

  S.push(&start);
  tree.addVertex(start.getName(), start.getData());

  while (!S.empty()) {
    const Vertex<int> * curr = S.top();
    S.pop();

    // check if curr was not discovered
    if (discovered.find(curr->getName()) == discovered.end()) {
      // label curr as discovered
      discovered.insert(curr->getName());

      for (auto& pair : curr->getNeighbours()) {
        Vertex<T> * v = pair.second;
        if (!tree.contains(v->getName())) {
          S.push(v);
          tree.addVertex(v->getName(), v->getData());
          tree.addEdge(v->getName(), curr->getName());
        }
      }
    }
  }
  return tree;
}

} // namespace

#endif // !__DFSGRAPH_HPP__
