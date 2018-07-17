// Notice: in order to use decreaseKey, all keys must be unique.
// Maybe future implementation will fix that.

// TODO: enable identical keys to use decrease key

#ifndef __HEAP_HPP__
#define __HEAP_HPP__

#include <vector>
#include <unordered_map>
#include <algorithm>

namespace HeapUtil {

 template<class Key, class Compare = std::less<Key>>
class Node {
 public:
  Key key;

  explicit Node(const Key& key) : key(key) {
  }

  friend bool operator<(const Node& lhs, const Node& rhs) {
    return Compare()(lhs.key, rhs.key);
  }

  friend bool operator==(const Node& lhs, const Node& rhs) {
    return !Compare()(lhs.key, rhs.key) && !Compare()(rhs.key, lhs.key);
  }
};

} // HeapUtil

template<class Key, class Compare = std::less<Key>>
class Heap {
 public:
  explicit Heap() {
  }

  explicit Heap(const std::vector<Key>& items) {
    std::size_t pos = 0;
    keyToPos.reserve(items.size());
    heap.reserve(items.size());
    for (auto& key : items) {
      heap.push_back(HeapUtil::Node<Key, Compare>(key));
      keyToPos[key] = pos++;
    }
    heapify();
  }

  template<class Iterator>
  explicit Heap(Iterator first, Iterator last) {
    std::size_t pos = 0;
    while (first != last) {
      heap.push_back(HeapUtil::Node<Key, Compare>(*first));
      keyToPos[*first] = pos++;
      ++first;
    }

    heapify();
  }

  void pop() {
    using std::swap; // enable ADL

    if (heap.size() < 0) {
      throw std::out_of_range("Heap::pop: empty heap.\n");
    }
    
    swap(heap.front(), heap.back());
    keyToPos[heap.front().key] = 0;
    keyToPos.erase(heap.back().key);
    heap.pop_back();
    siftDown(0);
  }

  Key& top() {
    if (heap.size() == 0) {
      throw std::out_of_range("Heap::top: empty heap.\n");
    }

    return heap.front().key;
  }

  void push(const Key& k) {
    heap.push_back(HeapUtil::Node<Key, Compare>(k));
    keyToPos[k] = heap.size() - 1;
    siftUp(heap.size() - 1);
  }

  bool empty() const {
    return heap.empty();
  }

  void reserve(std::size_t n) {
    heap.reserve(n);
  }

  std::size_t size() const {
    return heap.size();
  }

  void decreaseKey(const Key& key, const Key& newVal) {
    if (keyToPos.find(key) == keyToPos.end() || Compare()(key, newVal)) return;

    std::size_t pos = keyToPos[key];
    keyToPos.erase(key);
    keyToPos[newVal] = pos;
    heap[pos] = HeapUtil::Node<Key, Compare>(newVal);
    
    siftUp(pos);
  }

 private:
  bool hasLeftSon(std::size_t pos) const {
    if (pos * 2 + 2 > heap.size()) {
      return false;
    }
    return true;
  }

  bool hasRightSon(std::size_t pos) const {
    if (pos * 2 + 3 > heap.size()) {
      return false;
    }
    return true;
  }

  HeapUtil::Node<Key, Compare>& getLeftSon(std::size_t pos) {
    assert(hasLeftSon(pos));
    return heap[pos * 2 + 1];
  }

  HeapUtil::Node<Key, Compare>& getRightSon(std::size_t pos) {
    assert(hasRightSon(pos));
    return heap[pos * 2 + 2];
  }

  // if second is true, first is the position of the node swaped.
  // else no swap was done.
  std::pair<std::size_t, bool> siftDownOnce(std::size_t pos) {
    using std::swap; // enable ADL

    if (!hasLeftSon(pos) && !hasRightSon(pos)) {
      return { 0, false };
    }

    // has at least left son
    std::size_t minPos = pos;
    if (getLeftSon(pos) < heap[minPos]) {
      minPos = pos * 2 + 1;
    }

    if (hasRightSon(pos) && getRightSon(pos) < heap[minPos]) {
      minPos = pos * 2 + 2;
    }

    if (minPos != pos) {
      keyToPos[heap[minPos].key] = pos;
      keyToPos[heap[pos].key] = minPos;
      swap(heap[minPos], heap[pos]);
      return { minPos, true };
    }
    return { 0, false };
  }

  void siftUp(std::size_t pos) {
    using std::swap; // enable ADL

    // (pos - 1) / 2 is the parent position
    while (pos != 0 && heap[pos] < heap[(pos - 1) / 2]) {
      keyToPos[heap[pos].key] = (pos - 1) / 2;
      keyToPos[heap[(pos - 1) / 2].key] = pos;
      swap(heap[pos], heap[(pos - 1) / 2]);
      pos = (pos - 1) / 2;
    }
  }

  void siftDown(std::size_t pos) {
    std::pair<std::size_t, bool> p;
    do {
      p = siftDownOnce(pos);
      pos = p.first;
    } while (p.second);
  }

  void heapify() {
    if (heap.size() == 0) return;

    for (std::size_t i = heap.size(); i--;) {
      siftDown(i);
    }
  }

  std::vector<HeapUtil::Node<Key, Compare>> heap;
  std::unordered_map<Key, std::size_t> keyToPos;

};


#endif // !__HEAP_HPP__