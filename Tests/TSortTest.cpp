// Google Test headeres
#include <gtest/gtest.h>

#include "TopologicalSortGraph.hpp"

using namespace Acamol;

namespace {

class DAGTest : public ::testing::Test {
 protected:
  void SetUp() override {
    for (int i = 0; i <= 5; ++i) {
      g.addVertex(i, i);
    }

    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 5);
    g.addEdge(2, 4);
    g.addEdge(3, 4);
  }

  Graph<int> g;
};

class DAGFromWikiTest : public ::testing::Test {
protected:
  void SetUp() override {
    g.addVertex(7, 0);
    g.addVertex(5, 0);
    g.addVertex(3, 0);
    g.addVertex(11, 0);
    g.addVertex(8, 0);
    g.addVertex(2, 0);
    g.addVertex(9, 0);
    g.addVertex(10, 0);

    g.addEdge(5, 11);
    g.addEdge(7, 11);
    g.addEdge(7, 8);
    g.addEdge(3, 8);
    g.addEdge(3, 10);
    g.addEdge(11, 2);
    g.addEdge(11, 9);
    g.addEdge(11, 10);
    g.addEdge(8, 9);
  }

  Graph<int> g;
};

class CyclicTest : public ::testing::Test {
 protected:
  void SetUp() override {
    for (int i = 0; i <= 5; ++i) {
      g.addVertex(i, i);
    }

    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 5);
    g.addEdge(2, 4);
    g.addEdge(3, 4);
    g.addEdge(4, 0);
  }

  Graph<int> g;
};

TEST(TSortBasicEdgeCases, EmptyGraph) {
  Graph<int> g;
  auto l = topological_sort(g);
  ASSERT_EQ(0, l.size());
}

TEST(TSortBasicEdgeCases, OneElementOnly) {
  Graph<int> g;
  g.addVertex(0, 0);
  auto l = topological_sort(g);
  ASSERT_EQ(1, l.size());
}

TEST_F(CyclicTest, CyclicGraph) {
  auto l = topological_sort(g);
  ASSERT_EQ(0, l.size());

  // no longer cyclic
  g.removeEdge(4, 0);
  l = topological_sort(g);
  ASSERT_EQ(6, l.size());
}

TEST_F(DAGTest, TSortSimple) {
  std::list<Vertex<int>> l = topological_sort(g);

  ASSERT_EQ(6, l.size());
  int order[6];
  int i = 0;
  for (auto& v : l) {
    order[v.getName()] = i++;
  }

  for (i = 1; i < l.size(); ++i) {
    ASSERT_LT(order[0], order[i]);
  }
  
  ASSERT_LT(order[1], order[3]);
  ASSERT_LT(order[1], order[4]);

  ASSERT_LT(order[2], order[4]);
  ASSERT_LT(order[2], order[5]);
}

TEST_F(DAGFromWikiTest, TSortStillSimple) {
  auto l = topological_sort(g);

  ASSERT_EQ(8, l.size());
  int order[12];
  int i = 0;
  for (auto& v : l) {
    order[v.getName()] = i++;
  }

  ASSERT_LT(order[5], order[11]);

  ASSERT_LT(order[11], order[2]);
  ASSERT_LT(order[11], order[9]);
  ASSERT_LT(order[11], order[10]);

  ASSERT_LT(order[7], order[11]);
  ASSERT_LT(order[7], order[8]);

  ASSERT_LT(order[3], order[8]);
  ASSERT_LT(order[3], order[10]);

  ASSERT_LT(order[8], order[9]);
}

} // namespace
