#ifndef HUFFMANDECOMPRESSOR_H
#define HUFFMANDECOMPRESSOR_H

#define LETTER unsigned short
#define INVALID_CHARACTER 0x40
#define NOT_LEAF_BIT 0
#define LEAF_BIT 1
#define NOT_EOF 0x80

#include <fstream>
#include <iostream>
#include <string>
#include <bitset>
#include <stdlib.h>
#include <string.h>

#include "TreeNode.h"

using namespace std;

class HuffmanDecompressor {

    private:
        string inputFileName;
        string extension;
        string outputFileName;
        short letterSizeBits;
        short leftUncompressedBitsCount;
        short ignoredBitsCount;
        LETTER leftUncompressedBits;
        TreeNode* rootNode;
        bool verbose;
        
        void readMetaDataFromFile(ifstream*);
        void readDictionaryFromFile(ifstream*);
        void readDictionaryNode(ifstream*, TreeNode*, LETTER);
        void printDictionary(TreeNode*, string);
        void readFileAndDecompress(ifstream*);
        LETTER readABitFromFile(ifstream*);
        short writeABitToFile(ofstream*, LETTER, bool);
        string makeStringFromBits(LETTER, short);
        int readBitsFromFile(ifstream*, int);
        
    public:
        HuffmanDecompressor(string, bool);
        void decompress();
};

#endif
