
#include <vector>
#include <bitset>
#include <utility>
#include <memory>
#include <algorithm>
#include <unordered_map>


// basic binary tree
class tree {
public:
  std::shared_ptr<tree> left = nullptr, right = nullptr;
  char character;
  int frequency;

  bool leaf () { return left == nullptr && right == nullptr; }
};

// charmap is a map of characters to bit-vectors in an encoded tree
// i am typedefing it because this long templated typename is ugly
typedef std::unordered_map<char, std::vector<bool>> charmap;

// encoded_tree contains a string representation of a tree
// and a map of characters to bit-vectors
class encoded_tree {
public:
  std::string str;
  charmap map;

  encoded_tree (std::string s, charmap m) { str = s; map = m; }
};

// overload comparison operator to make trees sortable
bool operator> (const tree& left, const tree& right) {
  return left.frequency > right.frequency;
}

// convert a vector of char-frequency pairs into tree instances
std::vector<tree> treeify (std::vector<std::pair<char, int>> frequencies)
{
  std::vector<tree> vec;
  vec.reserve(frequencies.size());
  for (auto pair : frequencies) {
    tree t;
    t.character = pair.first;
    t.frequency = pair.second;
    vec.push_back(t);
  }

  return vec;
}

// construct a huffman tree out of a vector of char-frequency pairs
tree construct_huffman_tree(std::vector<std::pair<char, int>> frequencies)
{
  auto trees = treeify(frequencies);

  for (int size = trees.size(); size > 1; --size) {
    tree t;
    t.left = std::make_shared<tree>(trees[size - 2]);
    t.right = std::make_shared<tree>(trees[size - 1]);
    t.frequency = t.left->frequency + t.right->frequency;

    trees.pop_back(); trees.pop_back();

    // there's probably a more efficient way to insert a new
    // tree at the right position
    trees.push_back(t);
    std::sort(trees.begin(), trees.end(), std::greater<tree>());
  }

  return trees[0];
}

// delimiter character for converting trees to strings
char tree_delimiter ()
{
  std::bitset<8> bits("11111111");
  return (char)bits.to_ulong();
}

// convert a tree to a string
std::string tree_to_string (tree t)
{
  std::string str = "";
  if (t.leaf()) {
    str += t.character;
    return str;
  }

  str += tree_delimiter();
  str += tree_to_string(*t.left);
  str += tree_to_string(*t.right);

  return str;
}

// convert a tree into a map of characters to bit-vectors
void tree_to_map (tree t, charmap& map, std::vector<bool> path)
{
  if (t.leaf()) {
    map[t.character] = path;
    return;
  }

  std::vector<bool> leftpath, rightpath;

  leftpath.reserve(path.size() + 1); // pre-allocate memory for S P E E D
  leftpath.insert(leftpath.end(), path.begin(), path.end()); // copy current path
  leftpath.push_back(false);
  tree_to_map(*t.left, map, leftpath);

  rightpath.reserve(path.size() + 1);
  rightpath.insert(rightpath.end(), path.begin(), path.end());
  rightpath.push_back(true);
  tree_to_map(*t.right, map, rightpath);
}

// encode a huffman tree (see encoded_tree type defined above)
encoded_tree encode_huffman_tree (tree t)
{
  charmap map;
  std::vector<bool> path;
  tree_to_map(t, map, path);
  return encoded_tree(tree_to_string(t), map);
}

// parse an encoded tree string recursively
std::pair<tree, int> string_to_tree (std::string str, int index)
{
  tree t;

  if (str[index] != tree_delimiter()) {
    t.character = str[index];
    return std::make_pair(t, index + 1);
  }

  ++index;

  auto leftpair = string_to_tree(str, index);
  t.left = std::make_shared<tree>(leftpair.first);
  index = leftpair.second;

  auto rightpair = string_to_tree(str, index);
  t.right = std::make_shared<tree>(rightpair.first);
  index = rightpair.second;

  return std::make_pair(t, index);
}

// decode a huffman tree
tree decode_huffman_tree (std::string str)
{
  return string_to_tree(str, 0).first;
}
