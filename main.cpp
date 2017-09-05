#include <iostream>
#include <string.h>
#include <stdlib.h>
#include "HuffmanCompressor.h"
#include "HuffmanDecompressor.h"

void compressFile(std::string, int, bool);
void decompressFile(std::string, bool);

int main (int argc, char *argv[]) {
    if (argc == 1) {
        cout << "Missing parameters!" << endl;
        return 1;
    }
    
    bool debug = false;
    if (strcmp(argv[1], "compress") == 0) {
        // huffman compress file 1
        // huffman compress file 1 D
    
        if (argc != 4 && argc != 5) {
            std::cout << "Wrong number of parameters." << std::endl;
            return 1;
        }

        if (argc == 5 && strcmp(argv[4], "D") == 0) {
            debug = true;
        }
        compressFile(argv[2], atoi(argv[3]), debug);
        return 0;
        
    } else if (strcmp(argv[1], "decompress") == 0) {
        // huffman compress file
        // huffman compress file D
    
        if (argc != 3 && argc != 4) {
            std::cout << "Wrong number of parameters." << std::endl;
            return 1;
        }

        if (argc == 4 && strcmp(argv[3], "D") == 0) {
            debug = true;
        }
        decompressFile(argv[2], debug);
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
