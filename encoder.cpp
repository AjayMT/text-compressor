
#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>
#include <utility>
#include <algorithm>
#include <bitset>

#include "tree.cpp"


// pack a vector of bits into a vector of chars
std::vector<char> pack_bits (std::vector<bool> bits)
{
  std::vector<char> output;
  output.reserve(bits.size() / 8);

  int size = bits.size();
  int prev = 0;
  int cur = 0;
  for (; cur < size; ++cur) {
    if (cur - prev != 8) continue;

    std::bitset<8> bitset;
    for (int i = 0; i < 8; ++i) {
      if (bits[prev + i]) bitset[i] = 1;
      else bitset[i] = 0;
    }

    output.push_back((char)bitset.to_ulong());
    prev = cur;
  }

  if (size % 8 == 0) return output;

  std::bitset<8> last_bit;
  for (int i = 0; i < 8; ++i) {
    if (prev + i > size) {
      last_bit[i] = 0;
      continue;
    }

    if (bits[prev + i]) last_bit[i] = 1;
    else last_bit[i] = 0;
  }

  output.push_back((char)last_bit.to_ulong());

  return output;
}


int main (int argc, char *argv[])
{
  if (argc != 2) {
    std::cout << "Usage: encoder <tree-destination>" << std::endl;
    return 1;
  }

  std::string tree_file_name(argv[1]);

  // istreambuf_iterator reads all of stdin until EOF
  std::istreambuf_iterator<char> begin(std::cin), end;
  std::string input(begin, end);

  // construct frequency map
  std::unordered_map<char, int> frequency_map;
  for (auto c : input) ++frequency_map[c];

  // place key-value pairs into vector and sort by value
  std::vector<std::pair<char, int>> frequencies(frequency_map.begin(),
                                                frequency_map.end());
  std::sort(frequencies.begin(), frequencies.end(),
            [](auto& left, auto& right) {
              return left.second > right.second;
            });

  // construct and encode huffman tree
  tree huffman_tree = construct_huffman_tree(frequencies);
  encoded_tree et = encode_huffman_tree(huffman_tree);

  // assemble and write compressed output
  std::vector<bool> bits;
  for (auto c : input) {
    auto bs = et.map[c];
    for (auto b : bs) bits.push_back(b);
  }
  auto packed = pack_bits(bits);
  for (auto c : packed) std::cout << c;

  // write tree output
  std::ofstream tree_file;
  tree_file.open(tree_file_name);
  tree_file << et.str;
  tree_file.close();

  return 0;
}
