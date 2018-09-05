// Google test headers
#include <gtest/gtest.h>

#include "../Data Structures/WeightedGraph.hpp"
#include "../Graph/BellmanFordGraph.hpp"

using namespace Acamol::DataStructures;

namespace {

TEST(BFTest, SimpleCorrectness) {
  WeightedGraph<int> g;
  for (int i = 0; i <= 5; ++i) {
    g.addVertex(i, i);
  }

  g.addEdge(0, 1, 8);
  g.addEdge(0, 2, 10);
  g.addEdge(1, 3, 1);
  g.addEdge(4, 2, 1);
  g.addEdge(3, 2, -4);
  g.addEdge(3, 5, -1);
  g.addEdge(5, 4, -2);
  g.addEdge(2, 5, 2);

  auto SP = Acamol::Graph::bellman_ford(g, 0);
  auto graph = SP.first;
  auto distances = SP.second;
  ASSERT_EQ(5, graph.getNumOfEdges());
  double dist[] = { 0, 8, 5, 9, 5, 7 };
  for (int i = 0; i <= 5; ++i) {
    ASSERT_EQ(dist[i], distances[i]);
  }
}

TEST(BFTest, SimpleCorrectness2) {
  WeightedGraph<int> g;
  for (int i = 0; i <= 4; ++i) {
    g.addVertex(i, i);
  }

  g.addEdge(0, 1, -1);
  g.addEdge(0, 2, 4);
  g.addEdge(1, 2, 3);
  g.addEdge(1, 3, 2);
  g.addEdge(1, 4, 2);
  g.addEdge(3, 1, 1);
  g.addEdge(3, 2, 5);
  g.addEdge(4, 3, -3);

  auto SP = Acamol::Graph::bellman_ford(g, 0);
  auto graph = SP.first;
  auto distances = SP.second;
  ASSERT_EQ(4, graph.getNumOfEdges());
  double dist[] = { 0, -1, 2, -2, 1};
  for (int i = 0; i <= 4; ++i) {
    ASSERT_EQ(dist[i], distances[i]);
  }
}

TEST(BellmanFord_TDD, ExampleFromWiki) {
  WeightedGraph<int> g;
  for (int i = 1; i <= 6; ++i) {
    g.addVertex(i, i);
  }

  g.addEdge(1, 2, 7);
  g.addEdge(1, 6, 14);
  g.addEdge(1, 3, 9);
  g.addEdge(2, 4, 15);
  g.addEdge(2, 3, 10);
  g.addEdge(2, 1, 7);
  g.addEdge(3, 2, 10);
  g.addEdge(3, 1, 9);
  g.addEdge(3, 6, 2);
  g.addEdge(3, 4, 11);
  g.addEdge(4, 3, 11);
  g.addEdge(4, 2, 15);
  g.addEdge(4, 5, 6);
  g.addEdge(5, 4, 6);
  g.addEdge(5, 6, 9);
  g.addEdge(6, 1, 14);
  g.addEdge(6, 3, 2);
  g.addEdge(6, 5, 9);

  auto SP = Acamol::Graph::bellman_ford(g, 1);
  auto graph = SP.first;
  auto distances = SP.second;
  ASSERT_EQ(5, graph.getNumOfEdges());
  double dist[] = { 0, 7 ,9 ,20, 20, 11 };
  for (int i = 1; i <= 6; ++i) {
    ASSERT_EQ(dist[i - 1], distances[i]);
  }
}

TEST(BFTest, DetectNegativeCycle) {
  WeightedGraph<int> g;
  for (int i = 1; i <= 8; ++i) {
    g.addVertex(i, i);
  }

  g.addEdge(1, 2, 4);
  g.addEdge(1, 3, 4);
  g.addEdge(3, 6, -2);
  g.addEdge(3, 5, 4);
  g.addEdge(4, 1, 3);
  g.addEdge(4, 3, 2);
  g.addEdge(5, 4, 1);
  g.addEdge(5, 7, -2);
  g.addEdge(6, 5, -3);
  g.addEdge(6, 2, 3);
  g.addEdge(7, 6, 2);
  g.addEdge(7, 8, 2);
  g.addEdge(8, 5, -2);

  auto SP = Acamol::Graph::bellman_ford(g, 1);
  auto graph = SP.first;

  ASSERT_EQ(0, graph.getNumOfEdges());
  ASSERT_EQ(0, graph.getNumOfVertices());
}

} // namespace
