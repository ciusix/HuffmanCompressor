#ifndef COMPRESSORBASE_H
#define COMPRESSORBASE_H

#include <bitset>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include "TreeNode.h"
#include "FileReader.h"
#include "FileWriter.h"

class CompressorBase {

    protected:
        short letterSizeBits;
        std::string inputFileName;
        std::string extension;
        std::string outputFileName;
        bool verbose;
        FileReader *reader;
        FileWriter *writer;
        
    public:
        CompressorBase(std::string, bool);
        void initialize();
        void closeFiles();
        void printTree(TreeNode*, short);
        void printTree(TreeNode*, short, std::string);
        std::string makeStringFromBits(LETTER, short);
        std::string makeStringFromBits(std::vector<LETTER> bits);
};

#endif