#include "HuffmanCompressor.h"

HuffmanCompressor::HuffmanCompressor(std::string inputFileName, short letterSizeBits, bool verbose) : CompressorBase(inputFileName, verbose) {                  
    this->letterSizeBits = letterSizeBits;
}

void HuffmanCompressor::compress() {
    if (verbose) {
        std::cout << "Started compression" << std::endl;
    }

    initialize();
    makeTree(makeNodeList());
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

/**
 * Read file for the first time and make a letter->count pair list.
 * Returns vector of TreeNode objects with letter and count properties set.
 */
std::vector<TreeNode*> HuffmanCompressor::makeNodeList() {
    if (verbose) {
        std::cout << "Started reading file " << inputFileName << " for the first time to build a dictionary" << std::endl;
    }
    
    short bitsUntilEOF = INFINITY;
    LETTER letter;
    std::vector<TreeNode*> nodeList;
    int totalBits = 0;

    while (bitsUntilEOF == INFINITY || bitsUntilEOF >= letterSizeBits) {
        letter = reader->readBitsFromFile(letterSizeBits, &bitsUntilEOF);
        totalBits += letterSizeBits;

        bool letterInList = false;
        for (unsigned int i = 0; i < nodeList.size(); i++) {
            if (nodeList[i]->getLetter() == letter) {
                nodeList[i]->incCount();
                letterInList = true;
                break;
            }
        }
    
        if (!letterInList) {
            nodeList.push_back(new TreeNode(letter));
        }
    }

    uncompressedBitsCount = bitsUntilEOF;
    uncompressedBits = reader->readBitsFromFile(uncompressedBitsCount, &bitsUntilEOF);

    if (verbose) {
        std::cout << "Left uncompressed bits (" << uncompressedBitsCount << ") " << makeStringFromBits(uncompressedBits, uncompressedBitsCount) << std::endl;
        std::cout << "Finished reading file for the first time. Nodes: " << nodeList.size() << ". Content size: " << totalBits << " bits" << std::endl;
    } 

    return nodeList;
}

void HuffmanCompressor::makeTree(std::vector<TreeNode*> nodeList) {
    if (verbose) {
        std::cout << "Started making tree" << std::endl;
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
        std::cout << "Finished making tree" << std::endl;
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

    paddingZeroCount = writer->flush();

    if (verbose) {
        std::cout << "These " << paddingZeroCount << " trailing 0's at the end of " << outputFileName << " should be ignored" << std::endl;
        std::cout << "Ended reading file " << inputFileName << " for the second time" << std::endl;
        std::cout << "Ended writing compressed output to " << outputFileName << std::endl;
    }
}