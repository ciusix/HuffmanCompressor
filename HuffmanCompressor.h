#ifndef HUFFMANCOMPRESSOR_H
#define HUFFMANCOMPRESSOR_H

#define INVALID_CHARACTER 0x40
#define NOT_LEAF_BIT 0
#define LEAF_BIT 1

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <limits.h>
#include <bitset>
#include <vector>
#include <stdlib.h>
#include <string.h>

#include "CompressorBase.h"

struct LetterPair {
    LETTER letter;
    
    std::vector<LETTER> compressedLetterBits;
};

class HuffmanCompressor : CompressorBase {

    private:
        TreeNode* rootNode;
        std::vector<LetterPair*> pairList;
        int uncompressedBitsCount;
        LETTER uncompressedBits;
        int paddingZeroCount;

        std::vector<TreeNode*> makeNodeList();
        void makeTree(std::vector<TreeNode*>);
        void makePairFromNode(TreeNode*, std::vector<LETTER>);
        LetterPair* getPairForLetter(LETTER);
        void readFileAndCompress();

    public:
        HuffmanCompressor(std::string, short, bool);
        void compress();
        
};

#endif