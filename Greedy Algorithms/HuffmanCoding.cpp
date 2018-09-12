#include "HuffmanCoding.hpp"
#include "../Data Structures/Heap.hpp"
#include <limits>
#include <iostream>

namespace Acamol {

/** Node and derived classes methods **/
unsigned int Node::getFrequency() const {
  return frequency;
}

Node::Node(unsigned int f) : frequency(f) {
}

LeafNode::LeafNode(unsigned int f, char c) : Node(f), c(c) {
}

char LeafNode::getChar() const {
  return c;
}

InternalNode::InternalNode(Node * const l, Node * const r)
  : Node(l->getFrequency() + r->getFrequency()), left(l), right(r) {
}

InternalNode::~InternalNode() {
  delete left;
  delete right;
}

Node const * const InternalNode::getLeftSon() const {
  return left;
}

Node const * const InternalNode::getRightSon() const {
  return right;
}

struct NodeCompare {
  bool operator()(Node const * const lhs, Node const * const rhs) {
    return lhs->getFrequency() < rhs->getFrequency();
  }
};


/** HuffmanCodeEncoder methods **/
HuffmanCodeEncoder::HuffmanCodeEncoder(const std::string& sample)
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

HuffmanCodeEncoder::~HuffmanCodeEncoder() {
  delete root;
}

std::vector<unsigned int> HuffmanCodeEncoder::getFrequencies() {
  return frequencies;
}

void HuffmanCodeEncoder::printCode() const {
  for (auto& pair : codes) {
    if (pair.first == '\n') {
      std::cout << "newline: ";
    } else {
      std::cout << pair.first << ": ";
    }

    for (auto bit : pair.second) {
      std::cout << bit ? 1 : 0;
    }
    std::cout << std::endl;
  }
}

const CodeMap& HuffmanCodeEncoder::getCodeMap() const {
  return codes;
}

Node const * const HuffmanCodeEncoder::getHuffmanTree() const {
  return root;
}

const std::string& HuffmanCodeEncoder::getEncoded() const {
  return encoded;
}

Node * HuffmanCodeEncoder::generateTree() {
  Heap<Node *, NodeCompare> heap; // pointers to enable polymorphism

  // create the min heap by frequencies. at first, all nodes are leaves.
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
void HuffmanCodeEncoder::generateCode(Node const * const curr, const Code& prefix) {
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

void HuffmanCodeEncoder::generateEncoded(std::string sample) {
  for (std::size_t i = 0; i < sample.size(); ++i) {
    for (auto bit : codes[sample[i]]) {
      encoded += bit;
    }
  }
}


/** HuffmanCodeDecoder methods **/
HuffmanCodeDecoder::HuffmanCodeDecoder(Node const * const tree, const std::string& s) : decoded() {
  generateDecode(tree, s);
}

const std::string& HuffmanCodeDecoder::getDecoded() const {
  return decoded;
}

void HuffmanCodeDecoder::generateDecode(Node const * const root, const std::string& s) {
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

}  // namespace