/*
  Information about the algorithm and the problem it solves here:
  https://en.wikipedia.org/wiki/Huffman_coding

  Basically, A rip-off from this great code:
  https://rosettacode.org/wiki/Huffman_coding
  I wanted to experience coding it anyway. Also included a decoder, mainly for
  testing the encoder.

  This implementation encodes only std::strings, but it can be easily expanded to
  work with anything.
*/

#ifndef __HUFFMAN_CODING_HPP__
#define __HUFFMAN_CODING_HPP__

#include "../Data Structures/Heap.hpp"
#include <string>
#include <vector>
#include <limits>

 typedef std::vector<bool> Code;
 typedef std::map<char, Code> CodeMap;

 class Node {
   public:
    virtual ~Node() {
    }

    unsigned int getFrequency() const {
      return frequency;
    }
    
  protected:
    // just derived class can construct
    explicit Node(unsigned int f) : frequency(f) {

    }

   private:
    unsigned int frequency;

  };

 class LeafNode : public Node {
  public:
    explicit LeafNode(unsigned int f, char c) : Node(f), c(c) {
    }

    char getChar() const {
      return c;
    }

  private:
    const char c;
  };

 class InternalNode : public Node {
   public:
    explicit InternalNode(Node * const l, Node * const r) 
      : Node(l->getFrequency() + r->getFrequency()), left(l), right(r) {
    }

    ~InternalNode() {
      delete left;
      delete right;
    }

    Node const * const getLeftSon() const {
      return left;
    }

    Node const * const getRightSon() const {
      return right;
    }

   private:
    Node const * const left;
    Node const * const right;
  };


class HuffmanCodeEncoder {
 public:
  explicit HuffmanCodeEncoder(const std::string& sample) 
    : encoded(), frequencies(std::numeric_limits<unsigned char>::max(), 0) {
    for (auto& c : sample) {
      // signed char can have negative value if casted to integer,
      // so we shift the map such that characters are mapped from 0 and above
      ++frequencies[c + std::numeric_limits<char>::max()];
    }

    // generates the Huffman tree corresponds to the frequencies
    root = generateTree();
    generateCode(root, Code());
    generateEncoded(sample);
  }

  std::vector<unsigned int> getFrequencies() {
    return frequencies;
  }

  void printCode() const {
    for (auto& pair : codes) {
      if (pair.first == '\n') {
        std::cout <<  "newline: ";
      } else {
        std::cout << pair.first << ": ";
      }

      for (auto bit : pair.second) {
        std::cout << bit ? 1 : 0;
      }
      std::cout << std::endl;
    }
  }

  const CodeMap& getCodeMap() const {
    return codes;
  }

  Node const * const getHuffmanTree() const {
    return root;
  }

  const std::string& getEncoded() const {
    return encoded;
  }

 private:
  std::string encoded;
  std::vector<unsigned int> frequencies;
  CodeMap codes;
  Node * root;

  // Comparator for the min heap
  struct NodeCompare {
    bool operator()(Node const * const lhs, Node const * const rhs) {
      return lhs->getFrequency() < rhs->getFrequency();
    }
  };

  Node * generateTree() {
    Heap<Node *, NodeCompare> heap; // pointers to allow polymorphism

    // create the min heap by frequencies. at first, all nodes a leaves.
    for (std::size_t i = 0; i < frequencies.size(); ++i) {
      if (frequencies[i] > 0) {
        heap.push(new LeafNode(frequencies[i], static_cast<char>(i - std::numeric_limits<char>::max())));
      }
    }

    // this is the greedy step. each iteration we merge the two nodes with
    // the lowest freqencies, building the tree from bottom up.
    while (heap.size() > 1) {
      Node * left = heap.top();
      heap.pop();

      Node * right = heap.top();
      heap.pop();

      Node * merge = new InternalNode(left, right);
      heap.push(merge);
    }

    return heap.top();
  }

  // Traversing the Huffman tree in order to map characters to its code.
  // Every left turn becomes a '0' bit and right a '1' bit.
  void generateCode(Node const * const curr, const Code& prefix) {
    if (LeafNode const * const leaf = dynamic_cast<LeafNode const * const>(curr)) {
      // if it's a leaf node, we assign the character its code
      codes[leaf->getChar()] = prefix;
    } else if (InternalNode const * const inNode = dynamic_cast<InternalNode const * const>(curr)) {
      Code leftPrefix = prefix;
      leftPrefix.push_back(false);
      generateCode(inNode->getLeftSon(), leftPrefix);

      Code rightPrefix = prefix;
      rightPrefix.push_back(true);
      generateCode(inNode->getRightSon(), rightPrefix);
    }
  }

  void generateEncoded(std::string sample) {
    for (std::size_t i = 0; i < sample.size(); ++i) {
      for (auto& bit : codes[sample[i]]) {
        encoded += bit;
      }
    }
  }
};

class HuffmanCodeDecoder {
public:
  HuffmanCodeDecoder(Node const * const tree, const std::string& s) : decoded() {
    generateDecode(tree, s);
  }

  const std::string& getDecoded() const {
    return decoded;
  }

private:
  std::string decoded;
  
  void generateDecode(Node const * const root, const std::string& s) {
    Node const * node = root;
    for (std::size_t i = 0; i <= s.size();) {
      if (LeafNode const * const leaf = dynamic_cast<LeafNode const * const>(node)) {
        decoded += leaf->getChar();
        node = root;
      } else if (InternalNode const * const inNode = dynamic_cast<InternalNode const * const>(node)) {
          node = (s[i++] == 0) ? inNode->getLeftSon() : inNode->getRightSon();
      }
    }
  }
};

#endif // !__HUFFMAN_CODING_HPP__
