#ifndef TREENODE_H
#define TREENODE_H

#define LETTER unsigned short

#include <string>

class TreeNode {

    private:
        TreeNode* leftChild;
        TreeNode* rightChild;

        LETTER letter;
        int count;

    public:
        TreeNode();
        TreeNode(LETTER);
        TreeNode(TreeNode*, TreeNode*);
        
        LETTER getLetter();
        void setLetter(LETTER);
        void incCount();
        int getCount();
        TreeNode* getLeftChild();
        TreeNode* getRightChild();
        void setLeftChild(TreeNode*);
        void setRightChild(TreeNode*);
        bool isLeaf();
};

#endif