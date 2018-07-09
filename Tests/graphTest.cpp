#include <gtest/gtest.h> // Google Test headers

#include "../Graph/Graph.hpp"
#include <set>

namespace {

  TEST(GraphBasic, SimpleVertexInsertion) {
    Graph<int> g;
    ASSERT_FALSE(g.contains(0));
    g.addVertex(0, 22);
    ASSERT_TRUE(g.contains(0));
    ASSERT_EQ(g.getVertex(0).getData(), 22);
    ASSERT_EQ(1, g.getNumOfVertices());
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
      ASSERT_EQ(vPtr->getName(), 1); // only neighbour should be '1'
    }
  }

  struct bob {
    int a;
    bool b;
  };

  class compareBob {
   public:
    bool operator()(const struct bob& b1, const struct bob& b2) {
      return b1.a < b2.a;
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
    std::set<struct bob, compareBob> lBob;
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
      for (const Vertex<struct bob>* n : v.getNeighbours()) {
        // v should only have one edge, so this for loop should have
        // only one iteration
        ASSERT_EQ((v.getName() + 1) % SIZE_OF_BOB_TEST, n->getName());
      }
    }
  }

} // namespace