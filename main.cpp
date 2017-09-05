#include <iostream>
#include <string.h>
#include <stdlib.h>
#include "HuffmanCompressor.h"
#include "HuffmanDecompressor.h"

void compressFile(std::string, int, bool);
void decompressFile(std::string, bool);

int main (int argc, char *argv[]) {
    if (argc == 1) {
        std::cout << "Missing parameters! Usage:" << argv[0] << " [compress | decompress] [letterSize] file" << std::endl;
        return 1;
    }
    
    bool verbose = false;
    if (strcmp(argv[1], "compress") == 0) {
        if (argc != 4 && argc != 5) {
            std::cout << "Wrong number of parameters. Usage:" << argv[0] << " compress letterSize file" << std::endl;
            return 1;
        }

        if (argc == 5 && strcmp(argv[4], "D") == 0) {
            verbose = true;
        }
        compressFile(argv[2], atoi(argv[3]), verbose);
        return 0;
        
    } else if (strcmp(argv[1], "decompress") == 0) {
        if (argc != 3 && argc != 4) {
            std::cout << "Wrong number of parameters. Usage:" << argv[0] << " decompress file" << std::endl;
            return 1;
        }

        if (argc == 4 && strcmp(argv[3], "D") == 0) {
            verbose = true;
        }
        decompressFile(argv[2], verbose);
        return 0;
    } else {
        std::cout << "Unknown action (compress | decompress)" << std::endl;
        return 1;
    }
}

void compressFile(string inputFileName, int letterSizeBits, bool debugMode) {
    HuffmanCompressor* compressor = new HuffmanCompressor(inputFileName, letterSizeBits, debugMode, "");
    compressor->compress();
}

void decompressFile(string inputFileName, bool debugMode) {
    HuffmanDecompressor* decompressor = new HuffmanDecompressor(inputFileName, debugMode);
    decompressor->decompress();
}
