#include "HuffmanCompressor.h"

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
 
HuffmanCompressor::HuffmanCompressor(string inputFileName,
                                     string outputFileName,
                                     short letterSizeBits) {
                                     
	this->inputFileName = inputFileName;
	this->outputFileName = outputFileName;
	this->letterSizeBits = letterSizeBits;
	
	rootNode = new CompressorTreeNode(0);
}

void HuffmanCompressor::compress() {
	readFileAndMakeNodesList();
	makeTreeFromNodesList();
	makePairsFromTree();
	
	addMetaDataToFile();
	addDictionaryToFile();
	readFileAndCompress();
}

void HuffmanCompressor::readFileAndMakeNodesList() {
	ifstream inputFile;
	inputFile.open(inputFileName.c_str());

	int totalBits = 0;

	if (inputFile.is_open()) {
 	
 		LETTER letter = 0;
 		char currentBitInLetter = 0;
 		
 		LETTER bit;
 		bit = readABitFromFile(&inputFile);
 		
  		while (bit != INVALID_CHARACTER) {
    		
    		letter = (letter << 1) | bit;
    		currentBitInLetter++;
    		totalBits++;
    		
    		if (currentBitInLetter == letterSizeBits) {
    			// cout << bitset<16>(letter) << endl;
    			addLetterToNodesList(letter);
    			
    			letter = 0;
    			currentBitInLetter = 0;
    		}
    		
    		bit = readABitFromFile(&inputFile);
    	}
    	
    	leftBitsLength = (short)currentBitInLetter;
    	leftBits = letter;
    	
    	cout << "Left bits: " << (int)currentBitInLetter << " " << bitset<16>(letter) << endl;
    	cout << "Total bits: " << (totalBits + (int)currentBitInLetter)<< endl;
    	
  		inputFile.close(); 
  	}
	printf("\n");
	
	for (vector<CompressorTreeNode*>::iterator it = nodeList.begin(); it != nodeList.end(); ++it) {
		cout << bitset<16>((*it)->getLetter()) << " " << (*it)->getCount() << endl;
	}
	
}

void HuffmanCompressor::makeTreeFromNodesList() {
	while (nodeList.size() != 1) {
		CompressorTreeNode* firstSmallest = NULL;
		CompressorTreeNode* secondSmallest = NULL;
		
		int firstPos = -1;
		int secondPos = -1;
		
		int firstCount = INT_MAX;
		int secondCount = INT_MAX;
		
		for (unsigned int i = 0; i < nodeList.size(); i++) {
			if (nodeList[i]->getCount() < firstCount) {
				secondSmallest = firstSmallest;
				firstSmallest = nodeList[i];
				
				secondPos = firstPos;
				firstPos = i;
				
				secondCount = firstCount;
				firstCount = firstSmallest->getCount();
			} else if (nodeList[i]->getCount() < secondCount) {
				secondSmallest = nodeList[i];
				
				secondPos = i;
				
				secondCount = secondSmallest->getCount();
			}
		}
		
		// cout << firstSmallest->getCount() << " and " << secondSmallest->getCount() << endl;
		
		if (firstPos > secondPos) {
			nodeList.erase(nodeList.begin() + firstPos);
			nodeList.erase(nodeList.begin() + secondPos);
		} else {
			nodeList.erase(nodeList.begin() + secondPos);
			nodeList.erase(nodeList.begin() + firstPos);
		}
		
		nodeList.push_back(new CompressorTreeNode(firstSmallest, secondSmallest));
	}
	rootNode = nodeList[0];
}

void HuffmanCompressor::makePairsFromTree() {
	addPairForNode(rootNode, 0, 0);
	for (unsigned int i = 0; i < pairList.size(); i++) {
		cout << bitset<16>(pairList[i]->letter) << " " << bitset<16>(pairList[i]->compressedLetter) << " (" << pairList[i]->compressedLetterLength << ")" << pairList[i]->letter << "\n";
	}
}

HuffmanCompressor::LetterPair* HuffmanCompressor::getPairForLetter(LETTER letter) {
	for (unsigned int i = 0; i < pairList.size(); i++) {
		if (pairList[i]->letter == letter) {
			return pairList[i];
		}
	}
	return NULL;
}

void HuffmanCompressor::addPairForNode(CompressorTreeNode* node, LETTER prefix, int length) {
	if (node->getNodeOne() != NULL && node->getNodeTwo() != NULL) {
		length++;
		addPairForNode(node->getNodeOne(), (prefix << 1), length);
		addPairForNode(node->getNodeTwo(), ((prefix << 1) | 1), length);
	} else {
		LetterPair* newPair = new LetterPair();
		
		newPair->letter = node->getLetter();
		newPair->compressedLetter = prefix;
		newPair->compressedLetterLength = length;
		
		pairList.push_back(newPair);
	}
}

void HuffmanCompressor::readFileAndCompress() {
	ifstream inputFile;
	inputFile.open(inputFileName.c_str());

	ofstream outputFile;
	outputFile.open(outputFileName.c_str(), ios::out | ios::app);

	int totalBits = 0;

	if (inputFile.is_open() && outputFile.is_open()) {
 	
 		LETTER inputLetter = 0;
 		char currentBitInInputLetter = 0;

 		
 		LETTER bit;
 		bit = readABitFromFile(&inputFile);
 		
  		while (bit != INVALID_CHARACTER) {
    		
    		inputLetter = (inputLetter << 1) | bit;
    		currentBitInInputLetter++;
    		totalBits++;
    		
    		if (currentBitInInputLetter == letterSizeBits) {
    			LetterPair* pair = getPairForLetter(inputLetter);
    			
    			for (int i = 0; i < pair->compressedLetterLength; i++) {
    				writeABitToFile(&outputFile, (pair->compressedLetter) >> (pair->compressedLetterLength - 1 - i) , false);
    			}
    			
    			inputLetter = 0;
    			currentBitInInputLetter = 0;
    		}
    		
    		bit = readABitFromFile(&inputFile);
    	}
    	
    	short leftOverBits = writeABitToFile(&outputFile, 0, true);
    	cout << "These " << leftOverBits << " trailing 0's should be ignored" << endl;
    	
  		inputFile.close(); 
  		outputFile.close();
  	}
}

void HuffmanCompressor::addDictionaryToFile() {
	ofstream outputFile;
	outputFile.open(outputFileName.c_str(), ios::out | ios::app);
	
	addNodeToDictionary(rootNode, &outputFile);
	
	outputFile.close();
}

void HuffmanCompressor::addNodeToDictionary(CompressorTreeNode* node, ofstream* outputFile) {
	// 0 - not a leaf, 1 - leaf
	if (node->getNodeOne() != NULL && node->getNodeTwo() != NULL) {
		// cout << "0" << endl;
		writeABitToFile(outputFile, 0, false);
		addNodeToDictionary(node->getNodeOne(), outputFile);
		addNodeToDictionary(node->getNodeTwo(), outputFile);
	} else {
		writeABitToFile(outputFile, 1, false);
		for (int i = 0; i < letterSizeBits; i++) {
			writeABitToFile(outputFile, (node->getLetter()) >> (letterSizeBits - 1 - i), false);
		}
		// cout << "1 " << node->getLetter() << endl;
	}
}

void HuffmanCompressor::addMetaDataToFile() {
	ofstream outputFile;
	outputFile.open(outputFileName.c_str());
	
	short letterSize = letterSizeBits - 1;
	writeABitToFile(&outputFile, letterSize >> 3, false);
	writeABitToFile(&outputFile, letterSize >> 2, false);
	writeABitToFile(&outputFile, letterSize >> 1, false);
	writeABitToFile(&outputFile, letterSize, false);
	
	// Ignored bits count, will be filled later
	writeABitToFile(&outputFile, 0, false);
	writeABitToFile(&outputFile, 0, false);
	writeABitToFile(&outputFile, 0, false);
	writeABitToFile(&outputFile, 0, false);
	
	writeABitToFile(&outputFile, leftBitsLength >> 3, false);
	writeABitToFile(&outputFile, leftBitsLength >> 2, false);
	writeABitToFile(&outputFile, leftBitsLength >> 1, false);
	writeABitToFile(&outputFile, leftBitsLength, false);
	
	for (int i = 0; i < leftBitsLength; i++) {
		writeABitToFile(&outputFile, leftBits >> (leftBitsLength - 1 - i), false);
	}
	
	outputFile.close();
}

LETTER HuffmanCompressor::readABitFromFile(ifstream* file) {
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

short HuffmanCompressor::writeABitToFile(ofstream* file, LETTER bit, bool flush) {
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

void HuffmanCompressor::addLetterToNodesList(LETTER letter) {
	for (vector<CompressorTreeNode*>::iterator it = nodeList.begin(); it != nodeList.end(); ++it) {
		if ((*it)->getLetter() == letter) {
			(*it)->incCount();
			return;
		}
	}
	
	nodeList.push_back(new CompressorTreeNode(letter));
}