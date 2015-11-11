#include "HuffmanDecompressor.h"

/* 
 * |0000|0000|0000|00..00|00...00|00...00|
 *
 *  ^    ^    ^    ^      ^       ^
 *  |    |    |    |      |       |
 *  |    |    |    |      |       +- Content (? bits)
 *  |    |    |    |      |       
 *  |    |    |    |      +- Dictionary tree (? bits)
 *  |    |    |    +- Left uncompressed bits (? bits)
 *  |    |    +- Left uncompressed bits length (4 bits) 
 *  |    +- Ignored bits count (4 bits)
 *  +- Letter size (4 bits)
 *
 *
 */
 
HuffmanDecompressor::HuffmanDecompressor(string inputFileName, string outputFileName) {
	this->inputFileName = inputFileName;
	this->outputFileName = outputFileName;
}

void HuffmanDecompressor::decompress() {
	ifstream inputFile;
	inputFile.open(inputFileName.c_str());
	
	readMetaDataFromFile(&inputFile);
	
	inputFile.close();
}

void HuffmanDecompressor::readMetaDataFromFile(ifstream* inputFile) {
	letterSizeBits = 0;
	letterSizeBits = readABitFromFile(inputFile) << 3;
	letterSizeBits = letterSizeBits | (readABitFromFile(inputFile) << 2);
	letterSizeBits = letterSizeBits | (readABitFromFile(inputFile) << 1);
	letterSizeBits = letterSizeBits | readABitFromFile(inputFile);

	letterSizeBits += 1;
	
	ignoredBitsCount = 0;
	ignoredBitsCount = readABitFromFile(inputFile) << 3;
	ignoredBitsCount = ignoredBitsCount | (readABitFromFile(inputFile) << 2);
	ignoredBitsCount = ignoredBitsCount | (readABitFromFile(inputFile) << 1);
	ignoredBitsCount = ignoredBitsCount | readABitFromFile(inputFile);
	
	leftUncompressedBitsCount = 0;
	leftUncompressedBitsCount = readABitFromFile(inputFile) << 3;
	leftUncompressedBitsCount = leftUncompressedBitsCount | (readABitFromFile(inputFile) << 2);
	leftUncompressedBitsCount = leftUncompressedBitsCount | (readABitFromFile(inputFile) << 1);
	leftUncompressedBitsCount = leftUncompressedBitsCount | readABitFromFile(inputFile);
	
	leftUncompressedBits = 0;
	for (int i = 0; i < leftUncompressedBitsCount; i++) {
		leftUncompressedBits = leftUncompressedBits | (readABitFromFile(inputFile) << (leftUncompressedBitsCount - 1 - i));
	}
	
	cout << "Read letter size: " << letterSizeBits << " bits" << endl;
	cout << "Ignored bits count: " << ignoredBitsCount << endl;
	cout << "Read uncompressed bits count: " << leftUncompressedBitsCount << endl;
	cout << "Read uncompressed bits: " << bitset<16>(leftUncompressedBits) << endl;
	
}

LETTER HuffmanDecompressor::readABitFromFile(ifstream* file) {
	static short currentBitNumber = 0;
	static char c;
	if (currentBitNumber == 0) {
		if (!(file->get(c))) {
			return INVALID_CHARACTER;
		}
		currentBitNumber = 7;
	} else {
		currentBitNumber--;
	}
	return (c >> currentBitNumber) & 1;
}