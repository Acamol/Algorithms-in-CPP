#ifndef __DFSTREE__
#define __DFSTREE__

#include "Graph.hpp"
#include <unordered_set>
#include <stack>
#include <memory>

template<class T>
class DFSGraph : public Graph<T> {
public:
	typedef std::unique_ptr<DFSGraph<T>> DFSTreePtr;

	/*
	  Returns a DFSTreePtr which is a pointer to a graph representation of a
	  DFS tree whose root is the vertex "name". If no such vertex exists,
	  returns an empty tree.
	  Since DFSTreePtr is actually a uinque_ptr, it should be treated as such.

	  Notice that the returned graph is not actually a tree by the formal
	  definition. Each vertex in the returned graph points to it parent
	  (except the root), as illustrated in the example below.

	  Example:
	     Graph<int> g;
       g.addVertex(0, 2);
	     gaddVertex(1, 3);
	     g.addVertex(2, 5);
	     g.addEdge(0, 1);
	     g.addEdge(0, 2);
	     DFSTree tree = g.DFS(0);
	     tree->getData(0);

	  So a diagram of g can be thought like this:

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

		const Vertex<int>& vstart = this->getVertex(name);
		std::unordered_set<int> visited;
		std::stack<const Vertex<int>*> S;
		S.push(&vstart);
		tree->addVertex(vstart.getName(), vstart.getData());

		while (!S.empty()) {
			const Vertex<int> * curr = S.top();
			S.pop();
			// curr was not visited
			if (visited.find(curr->getName()) == visited.end()) {
				// label curr as visited
				visited.insert(curr->getName());

				for (auto v : curr->getNeighbours()) {
					S.push(v);
					tree->addVertex(v->getName(), v->getData());
					tree->addEdge(v->getName(), curr->getName());
				}
			}
		}
		return tree;
	}
};

#endif // !__DFSTREE__
