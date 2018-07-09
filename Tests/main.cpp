#include <gtest/gtest.h> // Google Test headers

int main(int argc, char ** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  RUN_ALL_TESTS();
  return 0;
}