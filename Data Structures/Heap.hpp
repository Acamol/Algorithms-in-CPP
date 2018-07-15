// Notice: in order to use decreaseKey, all keys must be unique.
// Maybe future implementation will fix that. In the meantime, if needed,
// it is possible to maintain a Key wrapper class which would ensure each key
// is unique.

#ifndef __HEAP_HPP__
#define __HEAP_HPP__

#include <vector>
#include <unordered_map>
#include <algorithm>

using std::swap; // enable ADL

template<class Key>
class Heap {
 public:
  explicit Heap(const std::vector<Key>& items) {
    std::size_t pos = 0;
    keyToPos.reserve(items.size());
    heap.reserve(items.size());
    for (auto& key : items) {
      heap.push_back(key);
      keyToPos[key] = pos++;
    }
    heapify();
  }

  template<class Iterator>
  explicit Heap(Iterator first, Iterator last) {
    std::size_t pos = 0;
    while (first != last) {
      heap.push_back(*first);
      keyToPos[*first] = pos++;
      ++first;
    }

    heapify();
  }

  void pop() {
    if (heap.size() < 0) {
      throw std::out_of_range("Heap::pop: empty heap.\n");
    }
    
    swap(heap.front(), heap.back());
    keyToPos[heap.front()] = 0;
    keyToPos.erase(heap.back());
    heap.pop_back();
    siftDown(0);
  }

  Key& top() {
    if (heap.size() == 0) {
      throw std::out_of_range("Heap::top: empty heap.\n");
    }

    return heap.front();
  }

  void push(const Key& k) {
    heap.push_back(k);
    keyToPos[k] = heap.size() - 1;
    siftUp(heap.size() - 1);
  }

  bool empty() const {
    return heap.empty();
  }

  void reserve(std::size_t n) {
    heap.reserve(n);
  }

  void decreaseKey(const Key& key, const Key& newVal) {
    if (keyToPos.find(key) == keyToPos.end() || key < newVal) return;

    std::size_t pos = keyToPos[key];
    keyToPos.erase(key);
    keyToPos[newVal] = pos;
    heap[pos] = newVal;
    
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

  Key& getLeftSon(std::size_t pos) {
    assert(hasLeftSon(pos));
    return heap[pos * 2 + 1];
  }

  Key& getRightSon(std::size_t pos) {
    assert(hasRightSon(pos));
    return heap[pos * 2 + 2];
  }

  // if second is true, first is the position of the node swaped.
  // else no swap was done.
  std::pair<std::size_t, bool> siftDownOnce(std::size_t pos) {
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
      keyToPos[heap[minPos]] = pos;
      keyToPos[heap[pos]] = minPos;
      swap(heap[minPos], heap[pos]);
      return { minPos, true };
    }
    return { 0, false };
  }

  void siftUp(std::size_t pos) {
    // (pos - 1) / 2 is the parent position
    while (pos != 0 && heap[pos] < heap[(pos - 1) / 2]) {
      keyToPos[heap[pos]] = (pos - 1) / 2;
      keyToPos[heap[(pos - 1) / 2]] = pos;
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

  std::vector<Key> heap;
  std::unordered_map<Key, std::size_t> keyToPos;
};

#endif // !__HEAP_HPP__
