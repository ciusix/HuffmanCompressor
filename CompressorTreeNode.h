#ifndef COMPRESSOTRTREENODE_H
#define COMPRESSOTRTREENODE_H

#define LETTER unsigned short

#include <string>
#include <fstream>
#include <iostream>

using namespace std;

class CompressorTreeNode {

    private:
    	CompressorTreeNode* nodeOne;
    	CompressorTreeNode* nodeTwo;

		LETTER letter;
		int count;

    public:
        CompressorTreeNode(LETTER);
        CompressorTreeNode(CompressorTreeNode*, CompressorTreeNode*);
        
        LETTER getLetter();
        void incCount();
        int getCount();
        CompressorTreeNode* getNodeOne();
        CompressorTreeNode* getNodeTwo();
};

#endif