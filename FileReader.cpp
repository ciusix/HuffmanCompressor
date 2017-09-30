#include "FileReader.h"

FileReader::FileReader(std::string fileName){
    std::cout << "Open file " << fileName << " for reading" << std::endl;

    file.open(fileName.c_str(), std::ios::binary | std::ios::in);

    // Assume, that file contains at least 3 bits
    // All 3 are loaded into "buffer"
    file.get(c1);
    file.get(c2);
    file.get(c3);

    // In the beginning we're looking at the first bit of a byte
    //
    // 7 6 5 4 3 2 1 0
    // ^

    currentBitIndex = sizeofinbits(char) - 1; // Set to 7
    leftUntilEOF = INFINITY;
}

LETTER FileReader::readABitFromFile(short *bitsLeft) {
    // If EOF is reached
    if (leftUntilEOF == 0) {
        *bitsLeft = 0;
        return (char)0;
    }
    
    if (currentBitIndex == -1) {
        c1 = c2;
        c2 = c3;
        if (!(file.get(c3)) && leftUntilEOF == INFINITY) {
            // New character cannot be read, so only c1 and c2 are left 
            leftUntilEOF = sizeofinbits(char) * 2;
        }
        currentBitIndex = sizeofinbits(char) - 1;
    }
    currentBitIndex--;
    
    if (leftUntilEOF != INFINITY) {
        leftUntilEOF--;
    }
    *bitsLeft = leftUntilEOF;

    // Add 1 because currentBitIndex was decremented, but bit at that index wasn't returned yet
    return (c1 >> (currentBitIndex + 1)) & 1;
}

int FileReader::readBitsFromFile(int bits, short *bitsLeft) {
    int result = 0;

    if ((unsigned int)bits > sizeofinbits(int)) {
        bits = sizeofinbits(int);
    }

    for (int i = 0; i < bits; i++) {
 
        if (bitsLeft == 0) {
            // Cannot read any more bits
            break;
        }
        result = result << 1;
        result = result | (readABitFromFile(bitsLeft) & 0x1);
    }
    return result;
}

std::string FileReader::readCharactersFromFile(int characters) {
    std::string text = "";
    short bitsLeft;
    char c;
    for (int i = 0; i < characters; i++) {
        c = (char)(readBitsFromFile(8, &bitsLeft));
        text = text + c;

        if (bitsLeft < 8) {
            break;
        }
    }
    return text;
}

void FileReader::close() {
    file.close();
}