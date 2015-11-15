#include "HuffmanDecompressor.h"

/* 
 * |00000000|00..00|0000|0000|0000|00..00|00...00|00...00|
 *
 *  ^        ^      ^    ^    ^    ^      ^       ^
 *  |        |      |    |    |    |      |       |
 *  |        |      |    |    |    |      |       +- Content (? bits)
 *  |        |      |    |    |    |      +- Dictionary tree (? bits)
 *  |        |      |    |    |    +- Left uncompressed bits (? bits)
 *  |        |      |    |    +- Left uncompressed bits length (4 bits) 
 *  |        |      |    +- Ignored (trailing zeros) bits count (4 bits)
 *  |        |      +- Letter size (4 bits)
 *  |        +- Extension in ASCII
 *  +- Extension length (8 bits)
 */
 
HuffmanDecompressor::HuffmanDecompressor(string inputFileName, bool debugMode) {
	this->inputFileName = inputFileName;
	
	this->debugMode = debugMode;
}

void HuffmanDecompressor::decompress() {
	if (debugMode) {
		cout << "Initiated decompression" << endl;
	}
	
	ifstream inputFile;
	inputFile.open(inputFileName.c_str());
	
	if (inputFile.is_open()) {
		readMetaDataFromFile(&inputFile);
		readDictionaryFromFile(&inputFile);
    	readFileAndDecompress(&inputFile);
    	
		inputFile.close();
	} else {
  		cout << "Error reading file " << inputFileName << endl;
  		cout << "Quitting..." << endl;
  		exit(1);
  	}
  	
  	if (debugMode) {
		cout << "Ended decompression" << endl;
	}
}

void HuffmanDecompressor::readMetaDataFromFile(ifstream* inputFile) {
	if (debugMode) {
		cout << "Started reading meta data" << endl;
	}	
	
	// Extension
	char extensionLength = 0;
	extension = "";
	inputFile->get(extensionLength);
	for (int i = 0; i < (int)extensionLength; i++) {
		char c;
		inputFile->get(c);
		extension = extension + c;
	}
	if (debugMode) {
		cout << "Output extension: " << extension << endl;
	}
	
	string::size_type idx = inputFileName.rfind('.');
	string currentExtension = "";
	if(idx != string::npos) {
    	currentExtension = inputFileName.substr(idx+1);
	}
	else {
    	currentExtension = "";
	}
	if (debugMode) {
		cout << "Current extension: " << currentExtension << endl;
	}
	this->outputFileName = inputFileName.substr(0, strlen(inputFileName.c_str()) - strlen(currentExtension.c_str()) - 1) + "-uncompressed." + extension;
	extensionLength = strlen(extension.c_str());
	if (debugMode) {
		cout << "Output file name: " << this->outputFileName << endl;
	}
	
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
	
	if (debugMode) {
		cout << "Ended reading meta data" << endl;
		cout << "Letter size: " << letterSizeBits << " bits" << endl;
		cout << "Ignored trailing bits count: " << ignoredBitsCount << endl;
		cout << "Uncompressed bits: " << makeStringFromBits(leftUncompressedBits, leftUncompressedBitsCount) << " (" << leftUncompressedBitsCount << ")" << endl;
	}	
}

void HuffmanDecompressor::readDictionaryFromFile(ifstream* file) {
	if (debugMode) {
		cout << "Started reading dictionary" << endl;
	}
    DecompressorTreeNode* rootNode = new DecompressorTreeNode();
    readDictionaryNode(file, rootNode, 0);
    rootTreeNode = rootNode;
    if (debugMode) {
    	cout << "Ended reading dictionary" << endl;
    	cout << "Dictionary:" << endl;
    	printDictionary(rootNode, 0);
    }
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

void HuffmanDecompressor::printDictionary(DecompressorTreeNode* node, int treeDepth) {
    if (node->getNodeOne() == NULL && node->getNodeTwo() == NULL) {
        cout << makeStringFromBits(node->getKey(), treeDepth) << " -> " << makeStringFromBits(node->getLetter(), letterSizeBits) << endl;
    } else {
        printDictionary(node->getNodeOne(), treeDepth + 1);
        printDictionary(node->getNodeTwo(), treeDepth + 1);
    }
}

void HuffmanDecompressor::readFileAndDecompress(ifstream* file) {
	if (debugMode) {
		cout << "Started decompressing " << inputFileName << " and writing output to " << outputFileName << endl;
	}
    LETTER bit = 0x1; // readABitFromFile(file);
 		
    ofstream outputFile;
	outputFile.open(outputFileName.c_str());
    
    if (outputFile.is_open()) {
		DecompressorTreeNode* currentTreeNode = rootTreeNode;
		while (bit != INVALID_CHARACTER) {
			if (currentTreeNode->isLeaf()) {
				for (int i = 0; i < letterSizeBits; i++) {
					writeABitToFile(&outputFile, currentTreeNode->getLetter() >> (letterSizeBits - 1 - i), false);
				}
				currentTreeNode = rootTreeNode;
			} else {
				bit = readABitFromFile(file);
			
				if (bit == 0) {
					currentTreeNode = currentTreeNode->getNodeOne();
				} else {
					currentTreeNode = currentTreeNode->getNodeTwo();
				}
				
			}
		}

		for (int i = 0; i < leftUncompressedBitsCount; i++) {
			writeABitToFile(&outputFile, leftUncompressedBits >> (leftUncompressedBitsCount - 1 - i), false);
		}
		// Flush last bits
		writeABitToFile(&outputFile, 0, true);
		
    	outputFile.close();
    } else {
  		cout << "Error writing file " << outputFileName << endl;
  		cout << "Quitting..." << endl;
  		exit(1);
  	}
  	
  	if (debugMode) {
  		cout << "Ended decompressing file" << endl;
  	}
}

LETTER HuffmanDecompressor::readABitFromFile(ifstream* file) {
	static short currentBitNumber = 0;
	static bool initialised = false;
	static char c1, c2, c3;
	static short leftUntilEOF = NOT_EOF;
	
	if (leftUntilEOF == 0) {
		return INVALID_CHARACTER;
	}
	
	if (!initialised) {
		initialised = true;
		file->get(c2);
		file->get(c3);
	}
	
	if (currentBitNumber == 0) {
		c1 = c2;
		c2 = c3;
		if (!(file->get(c3)) && leftUntilEOF == NOT_EOF) {
			leftUntilEOF = 16 - ignoredBitsCount;
		}
		currentBitNumber = 7;
	} else {
		currentBitNumber--;
	}
	
	if (leftUntilEOF != NOT_EOF) {
		leftUntilEOF--;
	}
	
	return (c1 >> currentBitNumber) & 1;
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

string HuffmanDecompressor::makeStringFromBits(LETTER bits, short length) {
	string result = "";
	for (short i = 0; i < length; i++) {
		if (bits & 0x1) {
			result = "1" + result;
		} else {
			result = "0" + result;
		}
		bits = bits >> 1;
	}
	return result;
}