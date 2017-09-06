#ifndef HUFFMANDECOMPRESSOR_H
#define HUFFMANDECOMPRESSOR_H

#define LETTER unsigned short
#define INVALID_CHARACTER 0x40
#define NOT_LEAF_BIT 0
#define LEAF_BIT 1
#define NOT_EOF 0x80

#include "CompressorBase.h"
#include <iostream>

class HuffmanDecompressor : CompressorBase {

    private:
        short leftUncompressedBitsCount;
        short ignoredBitsCount;
        LETTER leftUncompressedBits;
        TreeNode* rootNode;
        
        void readMetaDataFromFile();
        void readDictionaryFromFile();
        void readDictionaryNode(TreeNode*, LETTER);
        void readFileAndDecompress();

    public:
        HuffmanDecompressor(std::string, bool);
        void decompress();
};

#endif
