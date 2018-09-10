/*
  A generic (directed) graph represented with adjacency list. Each vertex is
  identified with a unique integer and can hold a data.

  This is by no means a complete data structure. Only the most basic
  operations are implemented, and only those I use in my implemantations.
*/

#ifndef __GRAPH_HPP__
#define __GRAPH_HPP__

#include <unordered_map>
#include <utility>

namespace Acamol {

template<class T>
class Vertex {
public:
  typedef std::unordered_map<int, Vertex<T>*> NeighboursList;

  explicit Vertex() {
  }

  explicit Vertex(int name, const T& data)
    : neighbours(), name(name), data(data), inDegree(0) {
  }

  virtual ~Vertex() {
  }

  void addNeighbour(Vertex<T>* neighbour) {
    neighbours[neighbour->getName()] = neighbour;
    ++(neighbour->inDegree);
  }

  int getName() const {
    return name;
  }

  const T& getData() const {
    return data;
  }

  const NeighboursList& getNeighbours() const {
    return neighbours;
  }

  NeighboursList& getNeighbours() {
    return neighbours;
  }

  bool removeNeighbour(Vertex<T>* neighbour) {
    if (neighbours.find(neighbour->getName()) != neighbours.end()) {
      neighbours.erase(neighbour->getName());
      --neighbour->inDegree;
      return true;
    }

    return false;
  }

  void clearEdges() {
    for (auto it = neighbours.begin(); it != neighbours.end();) {
      (*it).second->inDegree = 0;
      it = neighbours.erase(it);
    }
  }

  std::size_t getInDegree() const {
    return inDegree;
  }

  std::size_t getOutDegree() const {
    return neighbours.size();
  }

 protected:
  NeighboursList neighbours;

 private:
  int name;
  T data;
  int inDegree;
};

template<class T>
class Graph {
 public:
  typedef std::unordered_map<int, Vertex<T>> vMap;

  explicit Graph() : numEdges(0), vertices() {
  }

  /*
    Complexity: O(V+E)
  */
  Graph(const Graph& g) {
    numEdges = g.numEdges;
    vertices = g.vertices;
    for (auto& pair : vertices) {
      Vertex<T>& v = pair.second;
      for (auto& vertexPtr : v.getNeighbours()) {
        // the neighours are pointing to vertices in
        // g.vertices right now. this for loop fixes it.
        auto itToPtr = vertices.find(vertexPtr.second->getName());
        vertexPtr.second = &(itToPtr->second);
      }
    }
  }

  friend void swap(Graph<T>& first, Graph<T>& second) {
    using std::swap;

    swap(first.vertices, second.vertices);
    swap(first.numEdges, second.numEdges);
  }

  /*
    Complexity: O(V+E)
  */
  Graph& operator=(Graph g) {
    swap(*this, g);
    
    return *this;
  }

  Graph(Graph&& other) : vertices(std::move(other.vertices)), numEdges(other.numEdges) {
  }

  /*
    Complexity: constant
  */
  bool addVertex(int name, const T& data) {
    if (!contains(name)) {
      // there is no vertice with that name
      vertices[name] = Vertex<T>(name, data);
      return true;
    }

    return false;
  }

  /*
    Complexity: constant
  */
  bool addEdge(int name1, int name2) {
    if (contains(name1) && contains(name2)) {
      // both vertices exists
      Vertex<T>& v1 = vertices[name1];
      Vertex<T>& v2 = vertices[name2];

      // do not allow parallel edges - this is not a multigraph
      if (v1.getNeighbours().find(name2) == v1.getNeighbours().end()) {
        v1.addNeighbour(&v2);
        ++numEdges;
        return true;
      }
    }

    return false;
  }

  bool removeEdge(int name1, int name2) {
    if (contains(name1) && contains(name2)) {
      // both vertices exists
      Vertex<T>& v1 = vertices[name1];
      Vertex<T>& v2 = vertices[name2];
      if (v1.removeNeighbour(&v2)) {
        --numEdges;
        return true;
      }
    }

    return false;
  }

  /*
    Complexity: constant
  */
  const Vertex<T>& getVertex(int name) const {
    if (!contains(name)) {
      throw std::logic_error("\ngetVertex::Tried to get a vertex that does not exist.\n");
    }

    return vertices.find(name)->second;
  }

  /*
  Complexity: constant
  */
  Vertex<T>& getVertex(int name) {
    return const_cast<Vertex<T>&>(static_cast<const Graph*>(this)->getVertex(name));
  }

  /*
    Complexity: constant
  */
  const vMap& getVertices() const {
    return vertices;
  }

  /*
    Returns true if a vertex with identifier 'name' exists, and false otherwise.

    Complexity: constant
  */
  bool contains(int name) const {
    return (vertices.find(name) != vertices.end());
  }

  void clear() {
    vertices.clear();
    numEdges = 0;
  }

  void clearEdges() {
    for (auto& pair : vertices) {
      pair.second.clearEdges();
    }
    numEdges = 0;
  }

  std::size_t getNumOfVertices() const {
    return vertices.size();
  }

  std::size_t getNumOfEdges() {
    return numEdges;
  }

  /*
    Used to iterate over vertices in the graph. There is no specific order.
    Value cannot be changed through an iterator (like const_iterator).

    Example:
      Graph<int> g;
      ... adding some vertices ...
      for (auto& v : g) {
        std::cout << v.getData() << "\n";
      }
  */

  class Iterator {
   public:
    using difference_type = ptrdiff_t;
    using value_type = Vertex<T>;
    using reference = const Vertex<T>&;
    using pointer = const Vertex<T>*;
    using iterator_category = std::forward_iterator_tag;
    
    Iterator & operator++() {
      ++it;
      return *this;
    }

    bool operator==(const Iterator& it) const {
      _ASSERT(graph == it.graph);
      return (this->it == it.it);
    }

    bool operator!=(const Iterator& it) const {
      return !(*this == it);
    }

    const Vertex<T>& operator*() const {
      return it->second;
    }

    const Vertex<T>* operator->() const {
      return &(it->second);
    }

   private:
    friend class Graph<T>; // so Graph<T> will be able to call c'tor
    const Graph<T> * graph;
    typename vMap::iterator it;

    Iterator(const Graph<T> * graph, typename vMap::iterator it)
      : graph(graph), it(it) {
    }
  };
  
  Iterator begin() {
    return Iterator(this, vertices.begin());
  }

  Iterator end() {
    return Iterator(this, vertices.end());
  }

 protected:
  vMap vertices;
  std::size_t numEdges;
};

} // namespace

#endif // !__GRAPH_HPP__
