#include <iostream>
#include <string.h>
#include <stdlib.h>
#include "HuffmanCompressor.h"
#include "HuffmanDecompressor.h"

using namespace std;

void compressFile(string, int, bool);
void decompressFile(string, bool);

int main (int argc, char *argv[]) {
	if (argc == 1) {
		cout << "Missing parameters!" << endl;
		return 1;
	}
	
	if (strcmp(argv[1], "compress") == 0) {
	
		if (argc == 4) {
			compressFile(argv[2], atoi(argv[3]), false);
			return 0;
		} else if (argc == 5) {
			compressFile(argv[2], atoi(argv[3]), strcmp(argv[4], "D") == 0);
			return 0;
		} else {
			cout << "Wrong number of parameters." << endl;
			return 1;
		}
		
	} else if (strcmp(argv[1], "decompress") == 0) {
	
		if (argc == 3) {
			decompressFile(argv[2], false);
			return 0;
		} else if (argc == 4) {
			decompressFile(argv[2], strcmp(argv[3], "D") == 0);
			return 0;
		} else {
			cout << "Wrong number of parameters." << endl;
			return 1;
		}
		
	} else {
		cout << "Unknown action (compress | decompress)" << endl;
		return 1;
	}
}

void compressFile(string inputFileName, int letterSizeBits, bool debugMode) {
	HuffmanCompressor* compressor = new HuffmanCompressor(inputFileName, letterSizeBits, debugMode);
	compressor->compress();
}

void decompressFile(string inputFileName, bool debugMode) {
	HuffmanDecompressor* decompressor = new HuffmanDecompressor(inputFileName, debugMode);
	decompressor->decompress();
}