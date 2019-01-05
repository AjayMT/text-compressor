
# text-compressor
This is a very simple text compression algorithm based on [Huffman coding](https://en.wikipedia.org/wiki/Huffman_coding).

## Build
```sh
git clone http://github.com/AjayMT/text-compressor.git
cd text-compressor
make
```

## Usage
There are two components to this program: the encoder and the decoder.

### encoder
```sh
encoder <tree-destination>
```
`encoder` will read text from stdin, compress it and write it to stdout. Additionally, it will store the produced Huffman tree (which is required for decoding) in a file (`tree-destination`).

**Example:** This will compress `harry-potter.txt` into `harry-potter-compressed` and store the tree in `harry-potter-tree`.
```sh
encoder harry-potter-tree < harry-potter.txt > harry-potter-compressed
```

### decoder
```sh
decoder <tree-path>
```
`decoder` will read compressed text from stdin, decompress it (using data from the tree file at `tree-path`) and write it to stdout.

**Example:** This will decompress `harry-potter-compressed` into `harry-potter.txt` using data from `harry-potter-tree`.
```sh
decoder harry-potter-tree < harry-potter-compressed > harry-potter.txt
```

## Bugs and limitations
- `decoder` sometimes writes an extraneous character at the end. This is because `encoder` zeroes out leftover bits (since the compressed text may not fit into 8-bit characters perfectly), which are sometimes read as another character.
- Having to store the tree separately is cumbersome.
- Compression using Huffman coding is not particularly effective when all the characters in the text occur at the same (or very similar) frequency.
- The current implementation is not as efficient as it could (or should) be.

## Author(s)
- Ajay Tatachar (ajaymt2@illinois.edu)
