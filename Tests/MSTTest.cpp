#include <gtest/gtest.h>
#include "../Greedy Algorithms/MSTGraph.hpp"

namespace {

  TEST(MSTEdgeCases, EmptyTree) {
    MSTGraph<int> g;
    auto tree = g.getMST();
    ASSERT_EQ(0, tree->getWeight());
    ASSERT_EQ(0, tree->getNumOfEdges());
    ASSERT_EQ(0, tree->getNumOfVertices());
  }

  TEST(MSTEdgeCases, NoEdges) {
    MSTGraph<int> g;
    g.addVertex(0, 0);
    g.addVertex(1, 0);
    auto tree = g.getMST();
    ASSERT_EQ(0, tree->getNumOfVertices());
  }

  TEST(MSTEdgeCases, NotConnected) {
    MSTGraph<int> g;
    g.addVertex(0, 0);
    g.addVertex(1, 0);
    g.addVertex(2, 0);
    g.addEdge(0, 1, 20);
    auto tree = g.getMST();
    ASSERT_EQ(0, tree->getNumOfVertices());
  }

  TEST(MSTCorrectness, ExampeFromWiki) {
    MSTGraph<int> g;

    for (int i = 0; i <= 9; ++i) {
      g.addVertex(i, i);
    }

    // https://en.wikipedia.org/wiki/Minimum_spanning_tree#/media/File:Minimum_spanning_tree.svg
    g.addEdge(0, 1, 4);
    g.addEdge(0, 2, 1);
    g.addEdge(0, 3, 4);
    g.addEdge(1, 2, 5);
    g.addEdge(1, 4, 9);
    g.addEdge(1, 5, 9);
    g.addEdge(1, 6, 7);
    g.addEdge(2, 6, 9);
    g.addEdge(2, 3, 3);
    g.addEdge(3, 6, 10);
    g.addEdge(3, 9, 18);
    g.addEdge(4, 5, 2);
    g.addEdge(4, 7, 4);
    g.addEdge(4, 8, 6);
    g.addEdge(5, 7, 2);
    g.addEdge(5, 6, 8);
    g.addEdge(6, 9, 8);
    g.addEdge(6, 7, 9);
    g.addEdge(7, 8, 3);
    g.addEdge(7, 9, 9);
    g.addEdge(8, 9, 9); 

    auto tree = g.getMST();
    ASSERT_EQ(38, tree->getWeight());
  }

  TEST(MSTCorrectness, K5) {
    MSTGraph<int> g;

    for (int i = 0; i <= 4; ++i) {
      g.addVertex(i, i);
    }

    g.addEdge(0, 1, 24);
    g.addEdge(0, 2, 13);
    g.addEdge(0, 3, 13);
    g.addEdge(0, 4, 22);
    g.addEdge(1, 2, 22);
    g.addEdge(1, 3, 13);
    g.addEdge(1, 4, 13);
    g.addEdge(2, 3, 19);
    g.addEdge(2, 4, 14);
    g.addEdge(3, 4, 19);
    auto tree = g.getMST();

    ASSERT_EQ(52, tree->getWeight());
  }
} // namespace