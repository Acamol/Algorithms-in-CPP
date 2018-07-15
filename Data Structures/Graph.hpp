/*
  A generic (directed) graph represented with adjacency list. Each vertex is
  identified with a unique integer and can hold a data.

  This is by no means a complete data structure. Only the most basic
  operations are implemented, and only those I use in my implemantations.
*/

#ifndef __GRAPH_HPP__
#define __GRAPH_HPP__

#include <list>
#include <unordered_map>
#include <utility>

template<class T>
class Vertex {
public:
  typedef std::list<Vertex<T>*> NeighboursList;

  explicit Vertex() {
  }

  explicit Vertex(int name, const T& data)
    : neighbours(), name(name), data(data), inDegree(0) {
  }

  virtual ~Vertex() {
  }

  void addNeighbour(Vertex<T>* neighbour) {
    neighbours.push_back(neighbour);
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

  void removeNeighbour(Vertex<T>* neighbour) {
    for (auto it = neighbours.begin(); it != neighbours.end(); ++it) {
      if (*it == neighbour) {
        --neighbour->inDegree;
        neighbours.erase(it);
        break;
      }
    }
  }

  void clearEdges() {
    for (auto it = neighbours.begin(); it != neighbours.end();) {
      (*it)->inDegree = 0;
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
  std::list<Vertex<T>*> neighbours;

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
        auto itToPtr = vertices.find(vertexPtr->getName());
        vertexPtr = &(itToPtr->second);
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

  /*
    Complexity: constant
  */
  void addVertex(int name, const T& data) {
    if (!contains(name)) {
      // there is no vertice with that name
      vertices[name] = Vertex<T>(name, data);
    }

    // decide what to do if vertex with that name already exists
  }

  /*
    Complexity: constant
  */
  void addEdge(int name1, int name2) {
    if (contains(name1) && contains(name2)) {
      // both vertices exists
      Vertex<T>& v1 = vertices[name1];
      Vertex<T>& v2 = vertices[name2];
      v1.addNeighbour(&v2);
      ++numEdges;
    }

    // decide what to do if one of the vetices does not exist
  }

  void removeEdge(int name1, int name2) {
    if (contains(name1) && contains(name2)) {
      // both vertices exists
      Vertex<T>& v1 = vertices[name1];
      Vertex<T>& v2 = vertices[name2];
      v1.removeNeighbour(&v2);
      --numEdges;
    }
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
    if (!contains(name)) {
      throw std::logic_error("\ngetVertex::Tried to get a vertex that does not exist.\n");
    }

    return vertices.find(name)->second;
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
  size_t numEdges;

};

#endif // !__GRAPH_HPP__
