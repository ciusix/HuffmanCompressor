#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <fstream>
#include <iostream>

#define LETTER unsigned short

class FileWriter {

    private:
        std::ofstream file;
        short currentBitIndex;
        char c;

    public:
        FileWriter(std::string fileName);
        short writeABitToFile(LETTER);
        short flush();
        bool needsFlush();
        void close();
        
};

#endif