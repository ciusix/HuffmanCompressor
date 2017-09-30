#include "CompressorBase.h"

CompressorBase::CompressorBase(std::string inputFileName, bool verbose) {
    this->inputFileName = inputFileName;
    this->verbose = verbose;
}

void CompressorBase::initialize() {
    reader = new FileReader(inputFileName);
    writer = new FileWriter(outputFileName);
}

void CompressorBase::closeFiles() {
    reader->close();
    writer->close();
}

void CompressorBase::printTree(TreeNode* node, short letterSizeBits) {
    printTree(node, letterSizeBits, "");
}

void CompressorBase::printTree(TreeNode* node, short letterSizeBits, std::string key) {
    if (node->isLeaf()) {
        std::cout << key << " -> " << makeStringFromBits(node->getLetter(), letterSizeBits) << " count " << node->getCount() << std::endl;
    } else {
        printTree(node->getLeftChild(), letterSizeBits, key + "0");
        printTree(node->getRightChild(), letterSizeBits, key + "1");
    }
}

std::string CompressorBase::makeStringFromBits(LETTER bits, short length) {
    std::string result = "";
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

std::string CompressorBase::makeStringFromBits(std::vector<LETTER> bits) {
    std::string result = "";
    for (short i = 0; i < bits.size(); i++) {
        if (bits.at(i) == 1) {
            result = "1" + result;
        } else {
            result = "0" + result;
        }
    }
    return result;
}
