# HuffmanCompressor
Huffman compression written in C++ for "Information Theory".

(c) Justinas Rumševičius 2015

### Refactored in 2017.

Compressed file structure is as follows (0 is a single bit):
```
|00000000|00..00|0000|0000|0000|00..00|00...00|00...00|
 ^        ^      ^    ^    ^    ^      ^       ^
 |        |      |    |    |    |      |       |
 |        |      |    |    |    |      |       +- Content (? bits)
 |        |      |    |    |    |      +- Dictionary tree (? bits)
 |        |      |    |    |    +- Left uncompressed bits (? bits)
 |        |      |    |    +- Left uncompressed bits length (4 bits) 
 |        |      |    +- Ignored (trailing zeros) bits count (4 bits)
 |        |      +- Letter size (4 bits)
 |        +- Extension in ASCII (? bits)
 +- Extension length (8 bits)
```

*  Extension length (8 bits) - file extension is saved as plain text ASCII. These 8 bits keep extension length in bytes. Maximum extension length can be 255.
*  Extension in ASCII (? bits) - file extension saved as plain text ASCII.
*  Letter size (4 bits) - While compressing, file is read character by character. Character can be variable width - from 2 to 16 bits. Default is 8.
*  Ignored bits count (4 bits) - How many bits (zeros) at the end of file should be ignored. Zeros at the end of the file are used as padding so file size would be divisable by 8.
*  Left uncompressed bits length (4 bits) - How many bits were left uncompressed. This is always smaller than letter size. Bits are left uncompressed because they do not form a letter.
*  Left uncompressed bits (? bits) - Uncompressed bits.
*  Dictionary tree (? bits) - Dictionary about how data is encoded.
*  Content (? bits) - compressed content.