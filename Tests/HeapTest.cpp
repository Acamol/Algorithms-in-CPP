#include <gtest/gtest.h>

#include "Heap.hpp"

using namespace Acamol;

namespace {
   
TEST(HeapCorrectness, top_n_pop_empty_heap) {
  Heap<int> empty_heap;
  ASSERT_THROW(empty_heap.top(), std::out_of_range);
  ASSERT_THROW(empty_heap.pop(), std::out_of_range);
}

TEST(HeapCorrectness, heapify) {
  Heap<int> heap({ 5, 1, 3, 0, 2 });
}

TEST(HeapCorrectness, push) {
  Heap<int> heap({ 10, 2, 6, 0, 4 });
  heap.pop();
  heap.push(3);
}

TEST(HeapCorrectness, IteratorCtor) {
  std::vector<int> vec({ 10, 2, 6, 0, 4 });
  Heap<int> heap(vec.begin(), vec.end());
}

} // namespace