#include <iostream>
#include <string.h>
#include <stdlib.h>
#include "HuffmanCompressor.h"

int main (int argc, char *argv[]) {
    std::string inputFileName = argv[2];

    HuffmanCompressor* compressor = new HuffmanCompressor(inputFileName, (short)(atoi(argv[3])), true);
    compressor->compress();
}
