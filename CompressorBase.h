#ifndef COMPRESSORBASE_H
#define COMPRESSORBASE_H

#include <bitset>
#include <stdlib.h>
#include <string.h>
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
        void printTree(TreeNode*, std::string);
        std::string makeStringFromBits(LETTER, short);
};

#endif