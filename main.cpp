#include <iostream>
#include <string.h>
#include <stdlib.h>
#include "HuffmanCompressor.h"
#include "HuffmanDecompressor.h"

using namespace std;

void compressFile(string, int, bool, string);
void decompressFile(string, bool, string);

int main (int argc, char *argv[]) {
    if (argc == 1) {
        cout << "Missing parameters!" << endl;
        return 1;
    }
    
    if (strcmp(argv[1], "compress") == 0) {
        bool debug = false;
        string password = "";
    
    // huffman compress file 1
    // huffman compress file 1 D
    // huffman compress file 1   P 123
    // huffman compress file 1 D P 123
    
        if (argc == 4) {
            
        } else if (argc == 5) {
            if (strcmp(argv[4], "D") == 0) {
                debug = true;
                password = "";
            } else {
                cout << "Wrong parameters." << endl;
                return 1;
            }
        } else if (argc == 6) {
            if (strcmp(argv[4], "P") == 0) {
                debug = false;
                password = argv[5];
            } else {
                cout << "Wrong parameters." << endl;
                return 1;
            }
        } else if (argc == 7) {
            if (strcmp(argv[4], "D") == 0 && strcmp(argv[5], "P") == 0) {
                debug = true;
                password = argv[6];
            } else {
                cout << "Wrong parameters." << endl;
                return 1;
            }
        } else {
            cout << "Wrong number of parameters." << endl;
            return 1;
        }
        compressFile(argv[2], atoi(argv[3]), debug, password);
        return 0;
        
    } else if (strcmp(argv[1], "decompress") == 0) {
    bool debug = false;
        string password = "";
    
    // huffman compress file
    // huffman compress file D
    // huffman compress file   P 123
    // huffman compress file D P 123
    
        if (argc == 3) {
            
        } else if (argc == 4) {
            if (strcmp(argv[3], "D") == 0) {
                debug = true;
                password = "";
            } else {
                cout << "Wrong parameters." << endl;
                return 1;
            }
        } else if (argc == 5) {
            if (strcmp(argv[3], "P") == 0) {
                debug = false;
                password = argv[4];
            } else {
                cout << "Wrong parameters." << endl;
                return 1;
            }
        } else if (argc == 6) {
            if (strcmp(argv[3], "D") == 0 && strcmp(argv[4], "P") == 0) {
                debug = true;
                password = argv[5];
            } else {
                cout << "Wrong parameters." << endl;
                return 1;
            }
        } else {
            cout << "Wrong number of parameters." << endl;
            return 1;
        }
        decompressFile(argv[2], debug, password);
        return 0;
    } else {
        cout << "Unknown action (compress | decompress)" << endl;
        return 1;
    }
}

void compressFile(string inputFileName, int letterSizeBits, bool debugMode, string password) {
    HuffmanCompressor* compressor = new HuffmanCompressor(inputFileName, letterSizeBits, debugMode, password);
    compressor->compress();
}

void decompressFile(string inputFileName, bool debugMode, string password) {
    HuffmanDecompressor* decompressor = new HuffmanDecompressor(inputFileName, debugMode, password);
    decompressor->decompress();
}
