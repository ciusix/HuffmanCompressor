#  -Wall turns on most, but not all, compiler warnings
#
# for C++ define  CC = g++
CC = g++
CFLAGS = -Wall

# typing 'make' will invoke the first target entry in the file 
# (in this case the default target entry)
# you can name this target entry anything, but "default" or "all"
# are the most commonly used names by convention
#
default: huffman

build: clean huffman

# To create the executable file count we need the object files
# countwords.o, counter.o, and scanner.o:
#
huffman:  HuffmanCompressor.o HuffmanDecompressor.o
	$(CC) $(CFLAGS) -o huffman main.cpp HuffmanCompressor.o HuffmanDecompressor.o TreeNode.o

# To create the object file countwords.o, we need the source
# files countwords.c, scanner.h, and counter.h:
#


HuffmanCompressor.o:  HuffmanCompressor.cpp HuffmanCompressor.h TreeNode.o
	$(CC) $(CFLAGS) -c HuffmanCompressor.cpp

HuffmanDecompressor.o:  HuffmanDecompressor.cpp HuffmanDecompressor.h TreeNode.o FileReader.o FileWriter.o
	$(CC) $(CFLAGS) -c HuffmanDecompressor.cpp
    
TreeNode.o: TreeNode.cpp TreeNode.h
	$(CC) $(CFLAGS) -c TreeNode.cpp

FileReader.o: FileReader.cpp FileReader.h
	$(CC) $(CFLAGS) -c FileReader.cpp

FileWriter.o: FileWriter.cpp FileWriter.h
	$(CC) $(CFLAGS) -c FileWriter.cpp

clean:
ifeq ($(OS),Windows_NT)
	del *.o
	del *.gch
	del *.exe
else
	-rm *.o
	-rm *.gch
	-rm huffman
	-rm *.compressed
endif