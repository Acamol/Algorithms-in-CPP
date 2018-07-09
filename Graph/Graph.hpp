/*
  A generic (directed) graph represented with adjacency list. Each vertex is
  identified with a unique integer and can hold a data.

  This is by no means a complete data structure. Only the most basic
  operations are implemented, and only those I use in my algorithm
  implemantations.
*/

#ifndef __GRAPH_HPP__
#define __GRAPH_HPP__

#include <list>
#include <unordered_map>

template<class T>
class Vertex {
 public:
  typedef std::list<Vertex<T>*> NeighboursList;

  Vertex() {
  }

  Vertex(int name, const T& data) 
    : neighbours(), name(name), data(data) {
  }

  void addNeighbour(Vertex<T>* neighbour) {
    neighbours.push_back(neighbour);
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

 private:
  std::list<Vertex<T>*> neighbours;
  int name;
  T data;
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
    if (this != &g) {
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
  }

  /*
    Complexity: O(V+E)
  */
  Graph& operator=(const Graph& g) {
    if (this != &g) {
      vertices = g.vertices;
      for (auto& pair : vertices) {
        Vertex<T>& v = pair.second;
        for (Vertex<T> * vertexPtr : v.getNeighbours()) {
          auto itToPtr = vertices.find(vertexPtr->getName());
          vertexPtr = &(itToPtr->second);
        }
      }
    }
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
    Returns true if a vertex with identifier 'name' exists, and false otherwise.

    Complexity: constant
  */
  bool contains(int name) const {
    return (vertices.find(name) != vertices.end());
  }

  size_t getNumOfVertices() const {
    return vertices.size();
  }

  size_t getNumOfEdges() {
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

 private:
  size_t numEdges;
};

#endif // !__GRAPH_HPP__
