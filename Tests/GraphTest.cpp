// Google Test headers
#include <gtest/gtest.h> 

#include "Graph.hpp"
#include <set>

using namespace Acamol;

namespace {

TEST(GraphBasic, EmptyGraph) {
  Graph<int> g;
  ASSERT_EQ(0, g.getNumOfEdges());
  ASSERT_EQ(0, g.getNumOfVertices());
}

TEST(GraphBasic, IndegreeCheck) {
  Graph<int> g;
  for (int i = 0; i < 10; ++i) {
    g.addVertex(i, i);
  }

  for (auto& v : g) {
    ASSERT_EQ(0, v.getInDegree());
  }

  g.addEdge(2, 4);
  g.addEdge(2, 3);
  ASSERT_EQ(0, g.getVertex(2).getInDegree());
  ASSERT_EQ(1, g.getVertex(3).getInDegree());
  ASSERT_EQ(1, g.getVertex(4).getInDegree());
}

TEST(GraphBasic, SimpleVertexInsertion) {
  Graph<int> g;
  ASSERT_FALSE(g.contains(0));
  g.addVertex(0, 22);
  ASSERT_TRUE(g.contains(0));
  ASSERT_EQ(g.getVertex(0).getData(), 22);
  ASSERT_EQ(1, g.getNumOfVertices());
}

TEST(GraphBasic, CopyCtorAssignment) {
  Graph<int> g;
  g.addVertex(1, 0);
  g.addVertex(2, 0);
  Graph<int> copy = g; // copy c'tor
  
  ASSERT_EQ(2, g.getNumOfVertices());
  ASSERT_EQ(g.getNumOfVertices(), copy.getNumOfVertices());
  copy.addEdge(1, 2);
  const Vertex<int>& v1 = copy.getVertex(1);
  const Vertex<int> * adj = v1.getNeighbours().begin()->second;
  ASSERT_EQ(2, adj->getName());
  const Vertex<int>& v2 = g.getVertex(2);
  ASSERT_EQ(0, v2.getInDegree());
}

TEST(GraphBasic, SimpleEdgeInsertion) {
  Graph<int> g;
  g.addVertex(0, 0);
  g.addVertex(1, 0);
  g.addEdge(0, 1);
  ASSERT_TRUE(g.contains(0));
  ASSERT_TRUE(g.contains(1));
  ASSERT_EQ(1, g.getNumOfEdges());
  const Vertex<int>& v = g.getVertex(0);
  for (auto vPtr : v.getNeighbours()) {
    ASSERT_EQ(vPtr.second->getName(), 1); // only neighbour should be '1'
  }
}

struct bob {
  int a;
  bool b;

  friend bool operator<(const struct bob& first, const struct bob& second) {
    return first.a < second.a;
  }
};

#define SIZE_OF_BOB_TEST 100000
class GraphBobTest : public ::testing::Test {
protected:
  void SetUp() override {
    struct bob insertMe;

    for (int i = 0; i < SIZE_OF_BOB_TEST; ++i) {
      insertMe.a = i;
      insertMe.b = (i % 2 == 0);
      g.addVertex(i, insertMe);
    }
  }

  Graph<struct bob> g;
};

TEST_F(GraphBobTest, LotsOfVertexInsertion) {
  std::set<int> lNames;
  //auto compareBob = [](const struct bob& b1, const struct bob& b2) -> bool { return b1.a < b2.a; };
  auto lBob = std::set<struct bob>();
  for (auto v : g) {
    lNames.insert(v.getName());
    lBob.insert(v.getData());
  }
  int i = 0;
  std::set<int>::const_iterator it1 = lNames.begin();
  std::set<struct bob>::const_iterator it2 = lBob.begin();
  for (; it1 != lNames.end(); ++it1, ++it2, ++i) {
    ASSERT_EQ(i, *it1);
    const struct bob& testMe = *it2;
    ASSERT_EQ(i, testMe.a);
    ASSERT_EQ((i % 2 == 0), testMe.b);
  }
}

TEST_F(GraphBobTest, LotsOfEdgeInsertion) {
  // make it a circle
  for (int i = 0; i < SIZE_OF_BOB_TEST - 1; ++i) {
    g.addEdge(i, i + 1);
  }
  g.addEdge(SIZE_OF_BOB_TEST - 1, 0);

  for (const Vertex<struct bob>& v : g) {
    for (auto& n : v.getNeighbours()) {
      // v should only have one edge, so this for loop should have
      // only one iteration
      ASSERT_EQ((v.getName() + 1) % SIZE_OF_BOB_TEST, n.second->getName());
    }
  }
}

} // namespace
