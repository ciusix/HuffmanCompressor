#include "FileWriter.h"

FileWriter::FileWriter(std::string fileName) {
    std::cout << "Open file " << fileName << " for writing" << std::endl;

    file.open(fileName.c_str(), std::ios::binary | std::ios::out);

    currentBitIndex = 0;
    c = 0;
}

short FileWriter::writeABitToFile(LETTER bit) {
    if (currentBitIndex == 8) {
        flush();
    } else {
        c = c << 1;
    }
    
    c = (c | (bit & 0x1));    
    currentBitIndex++;
    return 0;
}

short FileWriter::flush() {
    if (needsFlush()) {
        file << ((char)(c << (8 - currentBitIndex)));
        c = 0;
        short leftBits = 8 - currentBitIndex;
        currentBitIndex = 0;
        return leftBits;
    } else {
        return 0;
    }
}

bool FileWriter::needsFlush() {
    return currentBitIndex != 0;
}

void FileWriter::close() {
    file.close();
}