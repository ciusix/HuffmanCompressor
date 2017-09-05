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

#include "TreeNode.h"

using namespace std;

class HuffmanCompressor {

struct LetterPair {
    LETTER letter;
    
      vector<LETTER> compressedLetterVector;
      int compressedLetterLength;
};

    private:
        string inputFileName;
        string outputFileName;
        short letterSizeBits;
        TreeNode* rootNode;
        vector<TreeNode*> nodeList;
        vector<LetterPair*> pairList;
        short leftBitsLength;
        LETTER leftBits;
        bool debugMode;
        string extension;
        short extensionLength;
        LETTER ignoredTrailingBits;
        string password;
        int letterInPassword;
        bool usePassword;
               
        void readFileAndMakeNodesList();
        void makeTreeFromNodesList();
        void makePairsFromTree();
        void addPairForNode(TreeNode*, vector<LETTER>, int, LETTER, int);
        LetterPair* getPairForLetter(LETTER letter);
        void readFileAndCompress();
        void addDictionaryToFile();
        void addNodeToDictionary(TreeNode*, ofstream*);
        void addMetaDataToFile();
        void writeIgnoredTrailingBits();
        LETTER readABitFromFile(ifstream*);
        short writeABitToFile(ofstream*, LETTER, bool);
        void addLetterToNodesList(LETTER);
        string makeStringFromBits(LETTER, short);
        string makeStringFromBits(vector<LETTER>, int);  

    public:
        HuffmanCompressor(string, short, bool, string);
        void compress();
        
};

#endif
