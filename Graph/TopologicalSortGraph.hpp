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

#include "../Data Structures/Graph.hpp"
#include "_Utility.hpp"

#include <unordered_set>
#include <list>
#include <stack>
#include <memory>

namespace Acamol {

template<class T>
std::list<Vertex<T>> topological_sort(const Graph<T>& graph) {
  // we don't want to ruin our graph, but some changes to the graph are
  // needed for the algorithm - i.e. removing edges.
  Graph<T> copy = graph;

  // find all sources in the graph. the definition of source is a vertex
  // which its indegree is 0.
  auto sources = internal::__findSources(copy);

  // the list that will hold the topological sort.
  std::list<Vertex<T>> result;

  // to indicate how many vertices in total were added to the sources list.
  // there's a topological sort iff at some point each vertex is or became
  // a source. so if there's a topological sort, k should be the number of
  // vertices in the graph in the end.
  std::size_t k = 0;
  while (!sources.empty()) {
    ++k;
    const Vertex<T>& source = *sources.front();
    sources.pop_front();
    // insert the current source to the topological order.
    result.push_back(Vertex<T>(source.getName(), source.getData()));

    // remove all outgoing edges from the current source
    const typename Vertex<T>::NeighboursList& sourceNeighbours = source.getNeighbours();
    for (auto it = sourceNeighbours.begin(); it != sourceNeighbours.end(); it = sourceNeighbours.begin()) {
      const Vertex<T>* v = (*it).second;
      // if v's indegree is 1, after removing 'source' from the graph, v's
      // indegree will be 0, which makes him a new source.
      if (v->getInDegree() == 1) {
        sources.push_back(v);
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
    result.clear();
  }
  return result;
}

}  // namespace

#endif // !__TOPOLOGICAL_SORT__
