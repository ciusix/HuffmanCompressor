#include "TreeNode.h"

TreeNode::TreeNode(){
    this->count = 0;
    
    this->leftChild = NULL;
    this->rightChild = NULL;
}

TreeNode::TreeNode(LETTER letter){
    this->letter = letter;
    this->count = 1;
    
    this->leftChild = NULL;
    this->rightChild = NULL;
}

TreeNode::TreeNode(TreeNode* leftChild, TreeNode* rightChild){
    this->letter = 0;
    this->count = leftChild->getCount() + rightChild->getCount();
    
    this->leftChild = leftChild;
    this->rightChild = rightChild;
}
        
LETTER TreeNode::getLetter(){
    return letter;
}

void TreeNode::setLetter(LETTER letter){
    this->letter = letter;
}
        
void TreeNode::incCount(){
    count++;
}

int TreeNode::getCount(){
    return count;
}

TreeNode* TreeNode::getLeftChild() {
    return leftChild;
}

TreeNode* TreeNode::getRightChild() {
    return rightChild;
}

void TreeNode::setLeftChild(TreeNode* node) {
    this->leftChild = node;
}

void TreeNode::setRightChild(TreeNode* node) {
    this->rightChild = node;
}

bool TreeNode::isLeaf() {
    return leftChild == NULL && rightChild == NULL;
}