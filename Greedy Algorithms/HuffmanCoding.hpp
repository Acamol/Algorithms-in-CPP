/*
  Information about the algorithm and the problem it solves here:
  https://en.wikipedia.org/wiki/Huffman_coding

  Basically, A rip-off from this code:
  https://rosettacode.org/wiki/Huffman_coding
  I wanted to code it anyway. Also included a decoder, mainly for
  testing the encoder.

  This implementation encodes only std::strings, but it can be easily expanded to
  work with anything.
*/

#ifndef __HUFFMAN_CODING_HPP__
#define __HUFFMAN_CODING_HPP__

#include <string>
#include <vector>
#include <map>

namespace Acamol { namespace Greedy {

 typedef std::vector<bool> Code;
 typedef std::map<char, Code> CodeMap;

 class Node {
   public:
    virtual ~Node() {}
    unsigned int getFrequency() const;
    
  protected:
    // just derived class can construct
    explicit Node(unsigned int f);

   private:
    unsigned int frequency;

  };

 class LeafNode : public Node {
  public:
    explicit LeafNode(unsigned int f, char c);
    char getChar() const;

  private:
    const char c;
  };

 class InternalNode : public Node {
   public:
    explicit InternalNode(Node * const l, Node * const r);
    ~InternalNode();
    Node const * const getLeftSon() const;
    Node const * const getRightSon() const;

   private:
    Node const * const left;
    Node const * const right;
  };

 
class HuffmanCodeEncoder {
 public:
  explicit HuffmanCodeEncoder(const std::string& sample);

  // Returns an histogram of all chars by their frequencies.
  std::vector<unsigned int> getFrequencies();

  // Prints a list of characters used in 'sample' and its
  // corrospoinding Huffman code given by the encoder.
  void printCode() const;

  // Returns a map from chars to Huffman code.
  const CodeMap& getCodeMap() const;

  // Returns a Huffman Tree of 'sample'.
  Node const * const getHuffmanTree() const;

  // Returns the encoded string.
  const std::string& getEncoded() const;

 private:
  std::string encoded;
  std::vector<unsigned int> frequencies;
  CodeMap codes;
  Node * root; // root of Huffman tree

  Node * generateTree();
  void generateCode(Node const * const curr, const Code& prefix);
  void generateEncoded(std::string sample);
};

class HuffmanCodeDecoder {
public:
  HuffmanCodeDecoder(Node const * const tree, const std::string& s);
  const std::string& getDecoded() const;

private:
  std::string decoded;
  
  void generateDecode(Node const * const root, const std::string& s);
};

} }

#endif // !__HUFFMAN_CODING_HPP__
