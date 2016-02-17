#ifndef DECOMPRESSOTRTREENODE_H
#define DECOMPRESSOTRTREENODE_H

#define LETTER unsigned short

#include <string>
#include <fstream>
#include <iostream>

using namespace std;

class DecompressorTreeNode {

    private:
        DecompressorTreeNode* nodeOne;
        DecompressorTreeNode* nodeTwo;

        LETTER letter;
    public:
        DecompressorTreeNode();
        
        void setLetter(LETTER);
        LETTER getLetter();
        void setNodeOne(DecompressorTreeNode*);
        DecompressorTreeNode* getNodeOne();
        void setNodeTwo(DecompressorTreeNode*);
        DecompressorTreeNode* getNodeTwo();
        bool isLeaf();
};

#endif