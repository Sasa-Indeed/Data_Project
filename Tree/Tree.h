#ifndef _TREE_H_
#define _TREE_H_
#include <iostream>

template<typename ElementType>
class TreeType{
private:
    //Data Memebers

    class Node{
    public:
        ElementType data;
        Node* rightNode;
        Node* leftNode;
        //--- Node constructor
        Node()
            :rightNode(NULL),leftNode(NULL)
        {}

        Node(ElementType value)
            : Node(), data(value)
        {}
    };

    typedef Node* NodePointer;
    NodePointer rootNode;
    int height;
    int treeSize;
    //For copying the tree
    void copyTree(NodePointer& thisTreeNode, NodePointer toCopyTreeNode);

    //For Destroying the tree
    void destroyTree(NodePointer& thisTreeNode);
public:
    //Default constructor
    TreeType();

    //Destructor
    ~TreeType();

    //Copy constructor
    TreeType(const TreeType<ElementType> & toCopyTree);

    //Is empty method
    bool isEmpty();

};

//Copy method to copy the tree
template<typename ElementType>
void TreeType<ElementType>::copyTree(NodePointer& thisTreeNode, NodePointer toCopyTreeNode) {
    /*Checking if the node of the tree to copy
    from is not NUll and it is also the base statement
    to return*/
    if (toCopyTreeNode == NULL) {
        thisTreeNode = NULL;
    }else{
        thisTreeNode = new Node(toCopyTreeNode->data);
        copyTree(thisTreeNode->leftNode, toCopyTreeNode->leftNode);
        copyTree(thisTreeNode->rightNode, toCopyTreeNode->rightNode);
    }
}

template<typename ElementType>
void TreeType<ElementType>::destroyTree(NodePointer& thisTreeNode){
    /*Checking if the node is not Null
    and going the left subtree then the 
    right subtree*/
    if (thisTreeNode) {
        destroyTree(thisTreeNode->leftNode);
        destroyTree(thisTreeNode->rightNode);
        delete thisTreeNode;
        thisTreeNode = NULL;
    }
}

//Default Constructor
template<typename ElementType>
TreeType<ElementType>::TreeType()
:rootNode(NULL),height(0),treeSize(0)
{}

//Destructor
template<typename ElementType>
TreeType<ElementType>::~TreeType(){
    destroyTree(this->rootNode);
}

//Copy Constuctor
template<typename ElementType>
TreeType<ElementType>::TreeType(const TreeType<ElementType>& toCopyTree){
    copyTree(this->rootNode, toCopyTree.rootNode);
}

//Empty method
template<typename ElementType>
bool TreeType<ElementType>::isEmpty(){
    return this->rootNode == NULL;
}







#endif // !_TREE_H_

