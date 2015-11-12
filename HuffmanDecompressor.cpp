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
	readDictionaryFromFile(&inputFile);
    readFileAndDecompress(&inputFile);
    
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

void HuffmanDecompressor::readDictionaryFromFile(ifstream* file) {
    DecompressorTreeNode* rootNode = new DecompressorTreeNode();
    readDictionaryNode(file, rootNode, 0);
    rootTreeNode = rootNode;
    printDictionary(rootNode);
}

void HuffmanDecompressor::readDictionaryNode(ifstream* file, DecompressorTreeNode* node, LETTER key) {
    LETTER bit = readABitFromFile(file);
    if (bit == LEAF_BIT) {
        LETTER letter = 0;
        for (int i = 0; i < letterSizeBits; i++) {
            letter = letter << 1;
            letter = letter | readABitFromFile(file);
        }
        node->setLetter(letter);
        node->setKey(key);
    } else {
        DecompressorTreeNode* nodeOne = new DecompressorTreeNode();
        DecompressorTreeNode* nodeTwo = new DecompressorTreeNode();
        node->setNodeOne(nodeOne);
        node->setNodeTwo(nodeTwo);
        
        readDictionaryNode(file, nodeOne, (key << 1) | 0);
        readDictionaryNode(file, nodeTwo, (key << 1) | 1);
    }
}

void HuffmanDecompressor::printDictionary(DecompressorTreeNode* node) {
    if (node->getNodeOne() == NULL && node->getNodeTwo() == NULL) {
        cout << bitset<16>(node->getKey()) << " " << bitset<16>(node->getLetter()) << endl;
    } else {
        printDictionary(node->getNodeOne());
        printDictionary(node->getNodeTwo());
    }
}

void HuffmanDecompressor::readFileAndDecompress(ifstream* file) {
    LETTER bit = readABitFromFile(file);
 		
    ofstream outputFile;
	outputFile.open(outputFileName.c_str());
    
    DecompressorTreeNode* currentTreeNode = rootTreeNode;
  	while (bit != INVALID_CHARACTER) {
        if (currentTreeNode->isLeaf()) {
            for (int i = 0; i < letterSizeBits; i++) {
                writeABitToFile(&outputFile, currentTreeNode->getLetter() >> (letterSizeBits - 1 - i), false);
            }
            currentTreeNode = rootTreeNode;
        } else {
            if (bit == 0) {
                currentTreeNode = currentTreeNode->getNodeOne();
            } else {
                currentTreeNode = currentTreeNode->getNodeTwo();
            }
            bit = readABitFromFile(file);
        }
    }
    
    outputFile.close();
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

short HuffmanDecompressor::writeABitToFile(ofstream* file, LETTER bit, bool flush) {
	static short currentBitNumber = 0;
	static char c = 0;
	
	if (flush) {
		(*file) << ((char)(c << (8 - currentBitNumber)));
		return 8 - currentBitNumber;
	}
		
	if (currentBitNumber == 8) {
		(*file) << c;
		c = 0;
		currentBitNumber = 0;
	} else {
		c = c << 1;
	}
	
	c = (c | (bit & 0x1));	
	currentBitNumber++;
	return 0;
}