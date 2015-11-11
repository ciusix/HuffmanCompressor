#include <iostream>
#include <string.h>
#include <stdlib.h>
#include "HuffmanCompressor.h"
#include "HuffmanDecompressor.h"

using namespace std;

void compressFile(string, string, int);
void decompressFile(string, string);

int main (int argc, char *argv[]) {
	cout << "Main starting..." << endl;
	
	if (argc == 1) {
		cout << "Missing parameters!" << endl;
		return 1;
	}
	
	if (strcmp(argv[1], "compress") == 0) {
		if (argc != 5) {
			cout << "Wrong number of parameters." << endl;
			return 1;
		}
		
		cout << "Will compress file " << argv[2] << " to " << argv[3] << endl;
		compressFile(argv[2], argv[3], atoi(argv[4]));
	} else if (strcmp(argv[1], "decompress") == 0) {
		if (argc != 4) {
			cout << "Wrong number of parameters." << endl;
			return 1;
		}
		
		cout << "Will compress file " << argv[2] << " to " << argv[3] << endl;
		decompressFile(argv[2], argv[3]);
	}
}

void compressFile(string inputFileName, string outputFileName, int letterSizeBits) {
	HuffmanCompressor* compressor = new HuffmanCompressor(inputFileName, outputFileName, letterSizeBits);
	compressor->compress();
}

void decompressFile(string inputFileName, string outputFileName) {
	HuffmanDecompressor* decompressor = new HuffmanDecompressor(inputFileName, outputFileName);
	decompressor->decompress();
}