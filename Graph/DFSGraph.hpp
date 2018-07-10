/*
The definition of Breadth-First Search can be found here:
https://en.wikipedia.org/wiki/Breadth-first_search

The complexity is O(V+E).
*/

#ifndef __DFSGRAPH_HPP__
#define __DFSGRAPH_HPP__

#include "Graph.hpp"
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <memory>

template<class T>
class DFSGraph : public Graph<T> {
 public:
  typedef std::unique_ptr<DFSGraph<T>> DFSTreePtr;

  /*
    Returns a DFSTreePtr which is a pointer to a graph representation of a
    DFS tree whose root is the vertex 'name'. If no such vertex exists,
    returns an empty tree.
    Since DFSTreePtr is actually a uinque_ptr, it should be treated as such.

    Notice that the returned graph is not actually a tree by the formal
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
  DFSTreePtr DFS(int name) const {
    DFSTreePtr tree = std::make_unique<DFSGraph<T>>();
    if (!this->contains(name)) return tree;

    const Vertex<int>& start = this->getVertex(name);
    std::unordered_set<int> discovered;
    std::stack<const Vertex<int>*> S;

    S.push(&start);
    tree->addVertex(start.getName(), start.getData());

    while (!S.empty()) {
      const Vertex<int> * curr = S.top();
      S.pop();

      // check if curr was not discovered
      if (discovered.find(curr->getName()) == discovered.end()) {
        // label curr as discovered
        discovered.insert(curr->getName());

        for (auto v : curr->getNeighbours()) {
          if (!tree->contains(v->getName())) {
            S.push(v);
            tree->addVertex(v->getName(), v->getData());
            tree->addEdge(v->getName(), curr->getName());
          }
        }
      }
    }
    return tree;
  }
};

#endif // !__DFSGRAPH_HPP__
