#include "FileReader.h"

FileReader::FileReader(std::ifstream *file){
    this->file = file;

    // Assume, that file contains at least 3 bits
    // All 3 are loaded into "buffer"
    file->get(c1);
    file->get(c2);
    file->get(c3);

    // In the beginning we're looking at the first bit of a byte
    //
    // 7 6 5 4 3 2 1 0
    // ^

    currentBitNumber = sizeof(char) - 1;
    leftUntilEOF = INFINITY;
}

LETTER FileReader::readABitFromFile(short *bitsLeft) {
    // If EOF is reached
    if (leftUntilEOF == 0) {
        (*bitsLeft) = 0;
        return (char)0;
    }
    
    if (currentBitNumber == 0) {
        c1 = c2;
        c2 = c3;
        if (!(file->get(c3)) && leftUntilEOF == INFINITY) {
            // New character cannot be read, so only c1 and c2 are left 
            leftUntilEOF = sizeof(char) * 2;
        }
        currentBitNumber = sizeof(char) - 1;
    } else {
        currentBitNumber--;
    }
    
    if (leftUntilEOF != INFINITY) {
        leftUntilEOF--;
    }
    (*bitsLeft) = leftUntilEOF;

    return (c1 >> currentBitNumber) & 1;
}

int FileReader::readBitsFromFile(int bits) {
    int result = 0;
    short bitsLeft = INFINITY;
    for (int i = 0; i < bits; i++) {

        if (bitsLeft == 0) {
            // Cannot read any more bits
            break;
        }

        result = result << 1;
        result = result | (readABitFromFile(&bitsLeft) & 0x1);
    }
    return result;
}

std::string FileReader::readCharactersFromFile(int characters) {
    std::string text = "";
    char c;
    for (int i = 0; i < characters; i++) {
        c = (char)(readBitsFromFile(8));
        text = text + c;
    }
    return text;
}

void FileReader::close() {
    file->close();
}