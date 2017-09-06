#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <fstream>

#define LETTER unsigned short

class FileWriter {

    private:
        std::ofstream *file;
        short currentBitNumber;
        char c;

    public:
        FileWriter(std::ofstream *);
        short writeABitToFile(LETTER bit);
        short flush();
        bool needsFlush();
        void close();
        
};

#endif