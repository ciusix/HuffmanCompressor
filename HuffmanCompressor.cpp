#include "HuffmanCompressor.h"

HuffmanCompressor::HuffmanCompressor(std::string inputFileName, short letterSizeBits, bool verbose) : CompressorBase(inputFileName, verbose) {                  
    this->letterSizeBits = letterSizeBits;
}

void HuffmanCompressor::compress() {
    if (verbose) {
        std::cout << "Started compression" << std::endl;
    }

    initialize();
    readFileAndMakeNodesList();
    makeTreeFromNodesList();
    printTree(rootNode, letterSizeBits);
    
    if (verbose) {
        std::cout << "Started making pairs of dictionary values (short letters) for existing keys (letters)" << std::endl;
    }
    makePairFromNode(rootNode, std::vector<LETTER>());
    if (verbose) {
        std::cout << "Ended making pairs" << std::endl;
        // std::cout << "Pairs: " << std::endl;
        for (unsigned int i = 0; i < pairList.size(); i++) {
            // std::cout << makeStringFromBits(pairList[i]->letter, letterSizeBits) << " -> " << makeStringFromBits(pairList[i]->compressedLetterBits) << std::endl;
        }
    }

    readFileAndCompress();

    if (verbose) {
        std::cout << "Ended compression" << std::endl;
    }
}

void HuffmanCompressor::readFileAndMakeNodesList() {
    if (verbose) {
        std::cout << "Started reading file " << inputFileName << " for the first time to build a dictionary" << std::endl;
    }
    
    int totalBits = 0;

    short bitsUntilEOF = INFINITY;
    LETTER letter;

    while (bitsUntilEOF == INFINITY || bitsUntilEOF >= letterSizeBits) {
        letter = reader->readBitsFromFile(letterSizeBits, &bitsUntilEOF);
        addLetterToNodesList(letter);
    }

    uncompressedBitsCount = bitsUntilEOF;
    uncompressedBits = reader->readBitsFromFile(uncompressedBitsCount, &bitsUntilEOF);

    std::cout << "Left uncompressed bits " << makeStringFromBits(uncompressedBits, uncompressedBitsCount) << std::endl; 
}

void HuffmanCompressor::addLetterToNodesList(LETTER letter) {
    for (unsigned int i = 0; i < nodeList.size(); i++) {
        if (nodeList[i]->getLetter() == letter) {
            nodeList[i]->incCount();
            return;
        }
    }
    
    nodeList.push_back(new TreeNode(letter));
}

void HuffmanCompressor::makeTreeFromNodesList() {
    if (verbose) {
        std::cout << "Started making tree nodes" << std::endl;
    }

    while (nodeList.size() != 1) {
        int firstPos = -1;
        int secondPos = -1;
        
        int firstCount = INT_MAX;
        int secondCount = INT_MAX;
        
        for (unsigned int i = 0; i < nodeList.size(); i++) {
            if (nodeList[i]->getCount() < firstCount) {
                secondPos = firstPos;
                firstPos = i;
                
                secondCount = firstCount;
                firstCount = nodeList[i]->getCount();
            } else if (nodeList[i]->getCount() < secondCount) {                
                secondPos = i;
                
                secondCount = nodeList[i]->getCount();
            }
        }
        
        TreeNode* firstSmallest = nodeList[firstPos];
        TreeNode* secondSmallest = nodeList[secondPos];

        nodeList.erase(nodeList.begin() + std::max(firstPos, secondPos));
        nodeList.erase(nodeList.begin() + std::min(firstPos, secondPos));

        nodeList.push_back(new TreeNode(firstSmallest, secondSmallest));
    }
    rootNode = nodeList[0];
    
    if (verbose) {
        std::cout << "Ended making tree nodes" << std::endl;
    }
}

void HuffmanCompressor::makePairFromNode(TreeNode* node, std::vector<LETTER> vectorOfPath) {
    if (node->isLeaf()) {
        LetterPair* pair = new LetterPair();
                        
        pair->letter = node->getLetter();
        pair->compressedLetterBits = vectorOfPath;
        
        pairList.push_back(pair);
    } else {
        std::vector<LETTER> leftVector(vectorOfPath);
        std::vector<LETTER> rightVector(vectorOfPath);

        leftVector.push_back(0);
        rightVector.push_back(1);

        makePairFromNode(node->getLeftChild(), leftVector);
        makePairFromNode(node->getRightChild(), rightVector);
    }
}

LetterPair* HuffmanCompressor::getPairForLetter(LETTER letter) {
    for (unsigned int i = 0; i < pairList.size(); i++) {
        if (pairList[i]->letter == letter) {
            return pairList[i];
        }
    }
    return NULL;
}

void HuffmanCompressor::readFileAndCompress() {
    if (verbose) {
        std::cout << "Started reading file " << inputFileName << " for the second time to compress" << std::endl;
        std::cout << "Will write output to " << outputFileName << std::endl;
    }

    short bitsUntilEOF = INFINITY;
    LETTER letter;
    letter = reader->readBitsFromFile(letterSizeBits, &bitsUntilEOF);

    while (bitsUntilEOF == INFINITY || bitsUntilEOF >= letterSizeBits) {
        letter = reader->readBitsFromFile(letterSizeBits, &bitsUntilEOF);
        
        LetterPair* pair = getPairForLetter(letter);
        
        for (int i = 0; i < pair->compressedLetterBits.size(); i++) {
            writer->writeABitToFile(pair->compressedLetterBits.at(i));
        }        
    }

    trailingZerosCount = writer->flush();

    if (verbose) {
        std::cout << "These " << trailingZerosCount << " trailing 0's at the end of " << outputFileName << " should be ignored" << std::endl;
        std::cout << "Ended reading file " << inputFileName << " for the second time" << std::endl;
        std::cout << "Ended writing compressed output to " << outputFileName << std::endl;
    }
}