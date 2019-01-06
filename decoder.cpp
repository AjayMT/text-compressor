
#include <iostream>
#include <iterator>
#include <vector>
#include <bitset>
#include <fstream>

#include "tree.cpp"


// unpack a string of characters into a bit-vector
std::vector<bool> unpack_bits (std::vector<char> input)
{
  std::vector<bool> bits;
  bits.reserve(input.size() * 8);
  for (char c : input) {
    std::bitset<8> bitset(c);
    for (int i = 0; i < 8; i++) bits.push_back((bool)bitset[i]);
  }

  return bits;
}


int main (int argc, char *argv[])
{
  if (argc != 2) {
    std::cout << "Usage: decoder <tree-path>" << std::endl;
    return 1;
  }

  std::string tree_file_name(argv[1]);

  // istreambuf_iterator reads all of stdin until EOF
  std::istreambuf_iterator<char> begin(std::cin), end;
  std::vector<char> input(begin, end);

  // read from tree file
  std::ifstream tree_file(tree_file_name);
  std::istreambuf_iterator<char> tree_begin(tree_file), tree_end;
  std::string tree_string(tree_begin, tree_end);

  // unpack input characters into bit-vector
  auto bits = unpack_bits(input);

  // decode tree
  tree huffman_tree = decode_huffman_tree(tree_string);

  // navigate tree and write output
  tree current = huffman_tree;
  for (bool b : bits) {
    if (b) current = *current.right;
    else current = *current.left;

    if (current.leaf()) {
      std::cout << current.character;
      current = huffman_tree;
    }
  }

  return 0;
}
