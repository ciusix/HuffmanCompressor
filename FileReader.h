#ifndef FILEREADER_H
#define FILEREADER_H

#include <fstream>
#include <string>
#include <iostream>

#define LETTER unsigned short
#define INFINITY -1
#define sizeofinbits(X) sizeof(X)*8

class FileReader {

    private:
        std::ifstream file;
        short currentBitIndex;
        char c1, c2, c3;
        short leftUntilEOF;

    public:
        FileReader(std::string fileName);
        LETTER readABitFromFile(short *);
        int readBitsFromFile(int, short *);
        std::string readCharactersFromFile(int);
        void close();
        
};

#endif