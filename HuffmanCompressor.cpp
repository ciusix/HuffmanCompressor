#include "HuffmanCompressor.h"

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
 
HuffmanCompressor::HuffmanCompressor(string inputFileName,
                                     short letterSizeBits,
                                     bool debugMode,
                                     string password) {
                                     
    this->inputFileName = inputFileName;
    this->letterSizeBits = letterSizeBits;
    
    this->debugMode = debugMode;
    
    string::size_type idx = inputFileName.rfind('.');
    if(idx != string::npos) {
        extension = inputFileName.substr(idx+1);
    }
    else {
        extension = "";
    }
    if (debugMode) {
        cout << "Current extension: " << extension << endl;
    }
    
    extensionLength = strlen(extension.c_str());
    
    if (extensionLength == 0) {
        this->outputFileName = inputFileName + ".compressed";
    } else {
        this->outputFileName = inputFileName.substr(0, strlen(inputFileName.c_str()) - strlen(extension.c_str()) - 1) + ".compressed";
    }
    
    usePassword = strlen(password.c_str()) != 0;
    this->password = password;
    letterInPassword = 0;
    
    if (debugMode) {
        cout << "Output file name: " << this->outputFileName << endl;
    }
}

void HuffmanCompressor::compress() {
    if (debugMode) {
        cout << "Initiated compression" << endl;
    }
    
    readFileAndMakeNodesList();
    makeTreeFromNodesList();
    makePairsFromTree();
    
    addMetaDataToFile();
    addDictionaryToFile();
    readFileAndCompress();
    writeIgnoredTrailingBits();
    
    if (debugMode) {
        cout << "Ended compression" << endl;
    }
}

void HuffmanCompressor::readFileAndMakeNodesList() {
    if (debugMode) {
        cout << "Started reading file " << inputFileName << " for the first time to build a dictionary" << endl;
    }
    
    ifstream inputFile;
    inputFile.open(inputFileName.c_str(), ios::binary | ios::in);

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
                addLetterToNodesList(letter);
                
                letter = 0;
                currentBitInLetter = 0;
            }
            
            bit = readABitFromFile(&inputFile);
        }
        
        leftBitsLength = (short)currentBitInLetter;
        leftBits = letter;
        
        if (debugMode) {
            cout << "Ended reading file for the first time" << endl;
            cout << "Left bits that do not form a letter: " << makeStringFromBits(letter, currentBitInLetter) << " (" << (int)currentBitInLetter << ")" << endl;
            cout << "Total bits read from file: " << totalBits << endl;
            cout << "Letters and their frequency: " << endl;
            for (unsigned int i = 0; i < nodeList.size(); i++) {
                cout << makeStringFromBits(nodeList[i]->getLetter(), letterSizeBits) << " (" << nodeList[i]->getCount() << ")" << endl;
            }
        }
        
          inputFile.close(); 
      } else {
          cout << "Error reading file " << inputFileName << endl;
          cout << "Quitting..." << endl;
          exit(1);
      }    
}

void HuffmanCompressor::makeTreeFromNodesList() {
    if (debugMode) {
        cout << "Started making tree nodes" << endl;
    }

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
    
    if (debugMode) {
        cout << "Ended making tree nodes" << endl;
    }
}

void HuffmanCompressor::makePairsFromTree() {
    if (debugMode) {
        cout << "Started making pairs of dictionary values (short letters) for existing keys (letters)" << endl;
    }
    
    // addPairForNode(rootNode, 0, 0);
    vector<LETTER> v;
    addPairForNode(rootNode, v, 0, 0, 0);
    
    if (debugMode) {
        cout << "Ended making pairs" << endl;
        cout << "Pairs: " << endl;
        for (unsigned int i = 0; i < pairList.size(); i++) {
            cout << makeStringFromBits(pairList[i]->letter, letterSizeBits)
                 << " -> "
                 << makeStringFromBits(pairList[i]->compressedLetterVector, pairList[i]->compressedLetterLength)
                 << endl;
        }
    }    
}

void HuffmanCompressor::addPairForNode(CompressorTreeNode* node, vector<LETTER> vectorOfLetters, int length, LETTER prefix, int prefixLength) {
    if (node->getNodeOne() != NULL && node->getNodeTwo() != NULL) {
        length++;
        
        if (prefixLength == 16) {
            vectorOfLetters.push_back(prefix);
            prefix = 0;
            prefixLength = 0;
        }
        prefixLength++;
        
        
        addPairForNode(node->getNodeOne(), vectorOfLetters, length, (prefix << 1), prefixLength);
        addPairForNode(node->getNodeTwo(), vectorOfLetters, length, ((prefix << 1) | 1), prefixLength);
    } else {
        LetterPair* newPair = new LetterPair();
                
        vectorOfLetters.push_back(prefix);
                
        newPair->letter = node->getLetter();
        newPair->compressedLetterVector = vectorOfLetters;
        newPair->compressedLetterLength = length;
        
        pairList.push_back(newPair);
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

void HuffmanCompressor::readFileAndCompress() {
    if (debugMode) {
        cout << "Started reading file " << inputFileName << " for the second time to compress" << endl;
        cout << "Will write output to " << outputFileName << endl;
    }

    ifstream inputFile;
    inputFile.open(inputFileName.c_str(), ios::binary | ios::in);

    ofstream outputFile;
    outputFile.open(outputFileName.c_str(), ios::binary | ios::out | ios::app);

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
                
                for (unsigned int j = 0; j < pair->compressedLetterVector.size()-1; j++) {
                    for (int i = 15; i >= 0; i--) {
                        writeABitToFile(&outputFile, (pair->compressedLetterVector.at(j)) >> i , false);
                    }
                }
                
                int leftToWrite = pair->compressedLetterLength - ((pair->compressedLetterVector.size() - 1) * 16);
                for (int i = 0; i < leftToWrite; i++) {
                    writeABitToFile(&outputFile, (pair->compressedLetterVector.at(pair->compressedLetterVector.size() - 1)) >> (leftToWrite - 1 - i) , false);
                }
                
                inputLetter = 0;
                currentBitInInputLetter = 0;
            }
            
            bit = readABitFromFile(&inputFile);
        }
        
        ignoredTrailingBits = writeABitToFile(&outputFile, 0, true);
        if (debugMode) {
            cout << "These " << ignoredTrailingBits << " trailing 0's at the end of " << outputFileName << " should be ignored" << endl;
        }
        
          inputFile.close(); 
          outputFile.close();
      } else {
          cout << "Error reading " << inputFileName << " or writing " << outputFileName << endl;
          cout << "Quitting..." << endl;
          exit(1);
      }
      
      if (debugMode) {
        cout << "Ended reading file " << inputFileName << " for the second time" << endl;
        cout << "Ended writing compressed output to " << outputFileName << endl;
    }
}

void HuffmanCompressor::writeIgnoredTrailingBits() {
    if (debugMode) {
        cout << "Started writing ignored trailing bits count (" << (int)ignoredTrailingBits << ") to " << outputFileName << endl;
    }

      char unreadBitsChar = 0;  
      unreadBitsChar = (unreadBitsChar | (letterSizeBits - 1)) << 4;
      unreadBitsChar = unreadBitsChar | (ignoredTrailingBits & 0x0F);
          
      if (usePassword) {
        unreadBitsChar = unreadBitsChar ^ password[0];
    }
          
      FILE * pFile;
      pFile = fopen (outputFileName.c_str(), "r+");
      if (pFile != NULL) {
          fseek (pFile , extensionLength + 1 , SEEK_SET );
          fputc (unreadBitsChar, pFile );
          fclose ( pFile );
      }  else {
          cout << "Error working with " << outputFileName << endl;
          cout << "Quitting..." << endl;
          exit(1);
      }
      if (debugMode) {
        cout << "Ended writing ignored trailing bits count" << endl;
    }
}

void HuffmanCompressor::addDictionaryToFile() {
    ofstream outputFile;
    outputFile.open(outputFileName.c_str(), ios::binary | ios::out | ios::app);
    
    addNodeToDictionary(rootNode, &outputFile);
    
    outputFile.close();
}

void HuffmanCompressor::addNodeToDictionary(CompressorTreeNode* node, ofstream* outputFile) {
    if (node->getNodeOne() != NULL && node->getNodeTwo() != NULL) {
        writeABitToFile(outputFile, NOT_LEAF_BIT, false);
        addNodeToDictionary(node->getNodeOne(), outputFile);
        addNodeToDictionary(node->getNodeTwo(), outputFile);
    } else {
        writeABitToFile(outputFile, LEAF_BIT, false);
        for (int i = 0; i < letterSizeBits; i++) {
            writeABitToFile(outputFile, (node->getLetter()) >> (letterSizeBits - 1 - i), false);
        }
    }
}

void HuffmanCompressor::addMetaDataToFile() {
    ofstream outputFile;
    outputFile.open(outputFileName.c_str(), ios::binary | ios::out);
    
    if (outputFile.is_open()) {
        outputFile << (char)extensionLength;
    
        for (short i = 0; i < extensionLength; i++) {
            outputFile << extension[i];
        }
    
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
    } else {
          cout << "Error writing " << outputFileName << endl;
          cout << "Quitting..." << endl;
          exit(1);
      }
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
        if (usePassword) {
            c = c ^ password[letterInPassword];
            letterInPassword = (letterInPassword + 1) % strlen(password.c_str());
        }
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

string HuffmanCompressor::makeStringFromBits(LETTER bits, short length) {
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

string HuffmanCompressor::makeStringFromBits(vector<LETTER> bitsVector, int length) {
    string result = "";
    int done = 0;
    
    for (unsigned int j = 0; j < bitsVector.size() - 1; j++) {
        result = result + makeStringFromBits(bitsVector.at(j), 16);
        result += " ";
        done += 16;
    }
    int left = length - done;
    result = result + makeStringFromBits(bitsVector.at(bitsVector.size() - 1), left);
    
    return result;
}
