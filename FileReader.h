#ifndef FILEREADER_H
#define FILEREADER_H

#include <fstream>
#include <string>

#define LETTER unsigned short
#define INFINITY -1

class FileReader {

    private:
        std::ifstream *file;
        short currentBitNumber;
        char c1, c2, c3;
        short leftUntilEOF;

    public:
        FileReader(std::ifstream *);
        LETTER readABitFromFile(short *);
        int readBitsFromFile(int);
        std::string readCharactersFromFile(int);
        void close();
        
};

#endif