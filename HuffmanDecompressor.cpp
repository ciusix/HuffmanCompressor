#include "HuffmanDecompressor.h"
 
HuffmanDecompressor::HuffmanDecompressor(string inputFileName, bool verbose) {
    this->inputFileName = inputFileName;
    
    this->verbose = verbose;
}

void HuffmanDecompressor::decompress() {
    if (verbose) {
        cout << "Initiated decompression" << endl;
    }
    
    ifstream inputFile;
    inputFile.open(inputFileName.c_str(), ios::binary | ios::in);
    
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
      
    if (verbose) {
        cout << "Ended decompression" << endl;
    }
}

void HuffmanDecompressor::readMetaDataFromFile(ifstream* inputFile) {
    if (verbose) {
        cout << "Started reading meta data" << endl;
    }    
    
    // Extension
    char extensionLength = 0;
    extension = "";
    inputFile->get(extensionLength);
    
    // Read extension
    char c;
    for (int i = 0; i < (int)extensionLength; i++) {
        inputFile->get(c);
        extension = extension + c;
    }
    if (verbose) {
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
    if (verbose) {
        cout << "Current extension: " << currentExtension << endl;
    }
    
    extensionLength = strlen(extension.c_str());
    
    this->outputFileName = inputFileName.substr(0, strlen(inputFileName.c_str()) - strlen(currentExtension.c_str()) - 1) + "-uncompressed";
    if (extensionLength != 0) {
        this->outputFileName += "." + extension;
    }
    
    if (verbose) {
        cout << "Output file name: " << this->outputFileName << endl;
    }
    
    letterSizeBits = readBitsFromFile(inputFile, 4);
    ignoredBitsCount = readBitsFromFile(inputFile, 4);
    leftUncompressedBitsCount = readBitsFromFile(inputFile, 4);
    leftUncompressedBits = readBitsFromFile(inputFile, leftUncompressedBitsCount);
    
    if (verbose) {
        cout << "Ended reading meta data" << endl;
        cout << "Letter size: " << letterSizeBits << " bits" << endl;
        cout << "Ignored trailing bits count: " << ignoredBitsCount << endl;
        cout << "Uncompressed bits: " << makeStringFromBits(leftUncompressedBits, leftUncompressedBitsCount) << " (" << leftUncompressedBitsCount << ")" << endl;
    }    
}

void HuffmanDecompressor::readDictionaryFromFile(ifstream* file) {
    if (verbose) {
        cout << "Started reading dictionary" << endl;
    }
    TreeNode* rootNode = new TreeNode();
    readDictionaryNode(file, rootNode, 0);
    this->rootNode = rootNode;
    if (verbose) {
        cout << "Ended reading dictionary" << endl;
        cout << "Dictionary:" << endl;
        printDictionary(rootNode, "");
    }
}

void HuffmanDecompressor::readDictionaryNode(ifstream* file, TreeNode* node, LETTER key) {
    LETTER bit = readABitFromFile(file);
    if (bit == LEAF_BIT) {
        LETTER letter = 0;
        for (int i = 0; i < letterSizeBits; i++) {
            letter = letter << 1;
            letter = letter | readABitFromFile(file);
        }
        node->setLetter(letter);
    } else {
        TreeNode* leftChild = new TreeNode();
        TreeNode* rightChild = new TreeNode();
        node->setLeftChild(leftChild);
        node->setRightChild(rightChild);
        
        readDictionaryNode(file, leftChild, (key << 1) | 0);
        readDictionaryNode(file, rightChild, (key << 1) | 1);
    }
}

void HuffmanDecompressor::printDictionary(TreeNode* node, string key) {
    if (node->getLeftChild() == NULL && node->getRightChild() == NULL) {
        cout << key << " -> " << makeStringFromBits(node->getLetter(), letterSizeBits) << endl;
    } else {
        printDictionary(node->getLeftChild(), key + "0");
        printDictionary(node->getRightChild(), key + "1");
    }
}

void HuffmanDecompressor::readFileAndDecompress(ifstream* file) {
    if (verbose) {
        cout << "Started decompressing " << inputFileName << " and writing output to " << outputFileName << endl;
    }
    LETTER bit = 0x1; // readABitFromFile(file);
         
    ofstream outputFile;
    outputFile.open(outputFileName.c_str(), ios::binary | ios::out);
    
    if (outputFile.is_open()) {
        TreeNode* currentTreeNode = rootNode;
        while (bit != INVALID_CHARACTER) {
            if (currentTreeNode->isLeaf()) {
                for (int i = 0; i < letterSizeBits; i++) {
                    writeABitToFile(&outputFile, currentTreeNode->getLetter() >> (letterSizeBits - 1 - i), false);
                }
                currentTreeNode = rootNode;
            } else {
                bit = readABitFromFile(file);
            
                if (bit == 0) {
                    currentTreeNode = currentTreeNode->getLeftChild();
                } else {
                    currentTreeNode = currentTreeNode->getRightChild();
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
      
      if (verbose) {
          cout << "Ended decompressing file" << endl;
      }
}

int HuffmanDecompressor::readBitsFromFile(ifstream* file, int bits) {
    int result = 0;
    for (int i = 0; i < bits; i++) {
        result = result << 1;
        result = result | (readABitFromFile(file) & 0x1);
    }
    return result;
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
