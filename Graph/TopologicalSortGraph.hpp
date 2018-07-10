/*
  The definition of topological sort can be found here:
  https://en.wikipedia.org/wiki/Topological_sorting

  In general, if a topological sort exists, this algorithm works
  as follows:
  1. while there's a source S in the graph:
    1.1 add S to the topological sort
    1.2 remove S and its outgoing edges 

  The implementation below deals with the case where the graph does not have a
  topological sort as well. It is described in the implementation.

  The complexity is O(V+E).
*/

#ifndef __TOPOLOGICAL_SORT__
#define __TOPOLOGICAL_SORT__

#include "Graph.hpp"
#include <unordered_set>
#include <list>
#include <stack>
#include <memory>

template<class T>
class TopologicalSortGraph : public Graph<T> {
 public:
   typedef std::unique_ptr<std::list<Vertex<T>>> TopologicalSortPtr;

   // Returns a topological sort of the vertices in the graph in a
   // (unique) pointer to a list.

   TopologicalSortPtr topologicalSort() const {
    // we don't want to ruin our graph, but some changes to the graph are
    // needed for the algorithm - i.e. removing edges.
    TopologicalSortGraph copy = *this;

    // find all sources in the graph. the definition of source is a vertex
    // which its indegree is 0.
    std::unique_ptr<std::list<const Vertex<T>*>> sources = copy.findSources();

    // the list that will hold the topological sort.
    TopologicalSortPtr result = std::make_unique<std::list<Vertex<T>>>();

    // to indicate how many vertices in total were added to the sources list.
    // there's a topological sort iff at some point each vertex is or became
    // a source. so if there's a topological sort, k should be the number of
    // vertices in the graph in the end.
    std::size_t k = 0;
    while (!sources->empty()) {
      ++k;
      const Vertex<T>& source = *sources->front();
      sources->pop_front();
      // insert the current source to the topological order.
      result->push_back(Vertex<T>(source.getName(), source.getData()));

      // remove all outgoing edges from the current source
      const Vertex<T>::NeighboursList& sourceNeighbours = source.getNeighbours();
      for (auto it = sourceNeighbours.begin(); it != sourceNeighbours.end(); it = sourceNeighbours.begin()) {
        const Vertex<T>* v = *it;
        // if v's indegree is 1, after removing 'source' from the graph, v's
        // indegree will be 0, which makes him a new source.
        if (v->getInDegree() == 1) {
          sources->push_back(v);
        }
        // this line effectivly changes 'sourceNeighbours' and the iterator.
        // this is why we need 'it = sourceNeighbours.begin()' in the for loop.
        // since each iteration 'sourceNeighbours' holds one less element,
        // the for loop will end eventually.
        copy.removeEdge(source.getName(), v->getName());
      }
    }

    // check if we have a topological sort.
    if (k != copy.getNumOfVertices()) {
      result->clear();
    }
    return result;
  }

 private:
  std::unique_ptr<std::list<const Vertex<T>*>> findSources() {
    auto sources = std::make_unique<std::list<const Vertex<T>*>>();
    for (auto& pair : this->vertices) {
      const Vertex<T>& v = pair.second;
      if (v.getInDegree() == 0) {
        sources->push_back(&pair.second);
      }
    }
    return sources;
  }
};
#endif // !__TOPOLOGICAL_SORT__
