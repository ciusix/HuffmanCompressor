#include "CompressorBase.h"

CompressorBase(std::string inputFileName, bool verbose) {
    this->inputFileName = inputFileName;
    this->letterSizeBits = letterSizeBits;
    this->verbose = verbose;
}

void CompressorBase::initialize() {
    ifstream inputFile;
    inputFile.open(inputFileName.c_str(), ios::binary | ios::in);

    reader = new FileReader(&inputFile);

    ofstream outputFile;
    outputFile.open(outputFileName.c_str(), ios::binary | ios::out);

    writer = new FileWriter(&outputFile);
}

void CompressorBase::closeFiles() {
    reader->close();
    writer->close();
}

void CompressorBase::printTree(TreeNode* node, int letterSizeBits, std::string key) {
    if (node->isLeaf()) {
        std::cout << key << " -> " << makeStringFromBits(node->getLetter(), letterSizeBits) << std::endl;
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