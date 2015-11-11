#ifndef HUFFMANDECOMPRESSOR_H
#define HUFFMANDECOMPRESSOR_H

#define LETTER unsigned short
#define INVALID_CHARACTER 0x40

#include <fstream>
#include <iostream>
#include <string>
#include <bitset>

using namespace std;

class HuffmanDecompressor {

    private:
		string inputFileName;
		string outputFileName;
		short letterSizeBits;
		short leftUncompressedBitsCount;
		short ignoredBitsCount;
		LETTER leftUncompressedBits;
		
		void readMetaDataFromFile(ifstream*);
		void readDictinaryFromFile(ifstream*);
		LETTER readABitFromFile(ifstream*);

    public:
        HuffmanDecompressor(string, string);
        void decompress();
};

#endif