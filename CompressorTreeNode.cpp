#include "CompressorTreeNode.h"

CompressorTreeNode::CompressorTreeNode(LETTER letter){
	this->letter = letter;
	this->count = 1;
	
	this->nodeOne = NULL;
	this->nodeTwo = NULL;
}

CompressorTreeNode::CompressorTreeNode(CompressorTreeNode* nodeOne, CompressorTreeNode* nodeTwo){
	this->letter = 0;
	this->count = nodeOne->getCount() + nodeTwo->getCount();
	
	this->nodeOne = nodeOne;
	this->nodeTwo = nodeTwo;
}
        
LETTER CompressorTreeNode::getLetter(){
	return letter;
}
        
void CompressorTreeNode::incCount(){
	count++;
}

int CompressorTreeNode::getCount(){
	return count;
}

CompressorTreeNode* CompressorTreeNode::getNodeOne() {
	return nodeOne;
}

CompressorTreeNode* CompressorTreeNode::getNodeTwo() {
	return nodeTwo;
}