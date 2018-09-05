//Google Test headers
#include <gtest/gtest.h>

#include "../Graph/DFSGraph.hpp"

using namespace Acamol::DataStructures;

namespace {

class DFSGraphTest : public ::testing::Test {
protected:
  /*
  tested graph:

                 +-+
            +----|0|----+
            |    +^+    |
            |     |     |
           +v+    |    +v+
      +----|1|    |    |2|----+
      |    +-+    |    +-+    |
      |     |     |     |     |
     +v+    |    +++    |    +v+
     |3|    +---->4<----+    |5|
     +-+         +^+         +-+
      |           |
      +-----------+

  */

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

  TEST_F(DFSGraphTest, DFSTree) {
    auto tree1 = Acamol::Graph::dfs(g, 0);
    // tree1 needs to be a spanning tree
    // if tree1 is indeed a spanning tree, then |E| = |V| - 1
    ASSERT_EQ(tree1.getNumOfVertices(), 6);
    ASSERT_EQ(tree1.getNumOfEdges(), 5);

    // vertex 5 has no adjacent vertices, so only vertex 5
    // should be in tree1 now
    tree1 = Acamol::Graph::dfs(g, 5);
    ASSERT_EQ(1, tree1.getNumOfVertices());
    ASSERT_EQ(0, tree1.getNumOfEdges());
    Graph<int>::Iterator it = tree1.begin();
    ASSERT_EQ(5, it->getName());
  }
} // namespace
