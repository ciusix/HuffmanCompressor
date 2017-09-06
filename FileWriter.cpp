#include "FileWriter.h"

FileWriter::FileWriter(std::ofstream *file) {
    this->file = file;
    currentBitNumber = 0;
    c = 0;
}

short FileWriter::writeABitToFile(LETTER bit) {
    if (currentBitNumber == 8) {
        flush();
    } else {
        c = c << 1;
    }
    
    c = (c | (bit & 0x1));    
    currentBitNumber++;
    return 0;
}

short FileWriter::flush() {
    (*file) << ((char)(c << (8 - currentBitNumber)));
    c = 0;
    short leftBits = 8 - currentBitNumber;
    currentBitNumber = 0;
    return leftBits;
}

bool FileWriter::needsFlush() {
    return currentBitNumber != 0;
}

void FileWriter::close() {
    file->close();
}