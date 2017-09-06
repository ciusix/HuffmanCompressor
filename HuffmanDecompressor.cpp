#include "HuffmanDecompressor.h"
 
HuffmanDecompressor::HuffmanDecompressor(std::string inputFileName, bool verbose) : CompressorBase(inputFileName, verbose) {
    
}

void HuffmanDecompressor::decompress() {
    if (verbose) {
        std::cout << "Starting decompression" << std::endl;
    }
    
    initialize();

    readMetaDataFromFile();
    readDictionaryFromFile();
    readFileAndDecompress();
    
    closeFiles();

    if (verbose) {
        std::cout << "Ended decompression" << std::endl;
    }
}

void HuffmanDecompressor::readMetaDataFromFile() {
    if (verbose) {
        std::cout << "Started reading meta data" << std::endl;
    }    
    
    int extensionLength = (int)(reader->readBitsFromFile(8));
    std::string extension = reader->readCharactersFromFile(extensionLength);
    
    letterSizeBits = reader->readBitsFromFile(4);
    ignoredBitsCount = reader->readBitsFromFile(4);
    leftUncompressedBitsCount = reader->readBitsFromFile(4);
    leftUncompressedBits = reader->readBitsFromFile(leftUncompressedBitsCount);
    
    if (verbose) {
        std::cout << "Ended reading meta data" << std::endl;
        std::cout << "Extension " << extension << std::endl;
        std::cout << "Letter size: " << letterSizeBits << " bits" << std::endl;
        std::cout << "Ignored trailing bits count: " << ignoredBitsCount << std::endl;
        std::cout << "Uncompressed bits: " << makeStringFromBits(leftUncompressedBits, leftUncompressedBitsCount) << " (" << leftUncompressedBitsCount << ")" << std::endl;
    }    
}

void HuffmanDecompressor::readDictionaryFromFile() {
    if (verbose) {
        std::cout << "Started reading dictionary" << std::endl;
    }

    TreeNode* rootNode = new TreeNode();
    readDictionaryNode(rootNode, 0);
    this->rootNode = rootNode;
    
    if (verbose) {
        std::cout << "Ended reading dictionary" << std::endl;
        std::cout << "Dictionary:" << std::endl;
        printTree(rootNode, "");
    }
}

void HuffmanDecompressor::readDictionaryNode(TreeNode* node, LETTER key) {
    LETTER bit = reader->readBitsFromFile(1);

    if (bit == LEAF_BIT) {
        LETTER letter = reader->readBitsFromFile(letterSizeBits);
        node->setLetter(letter);
    } else {
        TreeNode* leftChild = new TreeNode();
        TreeNode* rightChild = new TreeNode();
        node->setLeftChild(leftChild);
        node->setRightChild(rightChild);
        
        readDictionaryNode(leftChild, (key << 1) | 0);
        readDictionaryNode(rightChild, (key << 1) | 1);
    }
}

void HuffmanDecompressor::readFileAndDecompress() {
    LETTER bit = 0x1; // readABitFromFile(file);

    short bitsLeft = INFINITY;
    TreeNode* currentTreeNode = rootNode;
    while (bit != INVALID_CHARACTER) {
        if (currentTreeNode->isLeaf()) {
            for (int i = 0; i < letterSizeBits; i++) {
                writer->writeABitToFile(currentTreeNode->getLetter() >> (letterSizeBits - 1 - i));
            }
            currentTreeNode = rootNode;
        } else {
            bit = reader->readABitFromFile(&bitsLeft);
        
            if (bit == 0) {
                currentTreeNode = currentTreeNode->getLeftChild();
            } else {
                currentTreeNode = currentTreeNode->getRightChild();
            }

            if (bitsLeft == 0) {
                break;
            }
        }
    }

    for (int i = 0; i < leftUncompressedBitsCount; i++) {
        writer->writeABitToFile(leftUncompressedBits >> (leftUncompressedBitsCount - 1 - i));
    }

    // Flush last bits
    if (writer->needsFlush()) {
        writer->flush();
    }
}
