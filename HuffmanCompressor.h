#ifndef HUFFMANCOMPRESSOR_H
#define HUFFMANCOMPRESSOR_H

#define LETTER unsigned short
#define INVALID_CHARACTER 0x40
#define NOT_LEAF_BIT 0
#define LEAF_BIT 1

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <limits.h>
#include <bitset>

#include "CompressorTreeNode.h"

using namespace std;

class HuffmanCompressor {

struct LetterPair {
	LETTER letter;
	
  	LETTER compressedLetter;
  	short compressedLetterLength;
};

    private:
    	string inputFileName;
    	string outputFileName;
        short letterSizeBits;
        CompressorTreeNode* rootNode;
        vector<CompressorTreeNode*> nodeList;
        vector<LetterPair*> pairList;
        short leftBitsLength;
        LETTER leftBits;
        bool debugMode;
        string extension;
        short extensionLength;
        LETTER ignoredTrailingBits;
               
        void readFileAndMakeNodesList();
        void makeTreeFromNodesList();
        void makePairsFromTree();
        void addPairForNode(CompressorTreeNode*, LETTER, int);
        LetterPair* getPairForLetter(LETTER letter);
        void readFileAndCompress();
		void addDictionaryToFile();
		void addNodeToDictionary(CompressorTreeNode*, ofstream*);
		void addMetaDataToFile();
		void writeIgnoredTrailingBits();
        LETTER readABitFromFile(ifstream*);
        short writeABitToFile(ofstream*, LETTER, bool);
        void addLetterToNodesList(LETTER);
        string makeStringFromBits(LETTER, short); 

    public:
        HuffmanCompressor(string, short, bool);
        void compress();
        
};

#endif