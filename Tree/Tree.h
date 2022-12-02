//
// Created by Ahmed on 01/12/2022.
//

#ifndef _TREE_H_
#define _TREE_H_
#include <iostream>
#include <string>

template<typename ElementType>
class TreeType{
private:
    //Data Memebers

    class Node{
    public:
        ElementType data;
        Node* rightNode;
        Node* leftNode;
        Node* parentNode;
        //--- Node constructor

        Node()
                :rightNode(NULL),leftNode(NULL),parentNode(NULL)
        {
        }
    };

    typedef Node* NodePointer;
    NodePointer rootNode;

    int height;
    int treeSize;
    //For copying the tree
    void copyTree(NodePointer& thisTreeNode, NodePointer toCopyTreeNode);

    //For Destroying the tree
    void destroyTree(NodePointer& thisTreeNode);

    void insertTN(NodePointer& thisTreeNode, std::string expression, int index);

public:
    //Default constructor
    TreeType();

    //Destructor
    ~TreeType();

    //Copy constructor
    TreeType(const TreeType<ElementType> & toCopyTree);

    void insert(std::string expression, int index);

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
{
    rootNode = new TreeType::Node();
}

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

template<typename ElementType>
void TreeType<ElementType>::insertTN(NodePointer& thisTreeNode, std::string expression, int index) {
if(!(index > expression.length()) || !(index < 0)) {
    if (expression[index] == '(') {
        thisTreeNode->leftNode = new TreeType::Node();
        thisTreeNode->leftNode->parentNode = thisTreeNode;
        insertTN(thisTreeNode->leftNode, expression, index + 1);
    } else if (expression[index] == '+' || expression[index] == '-' || expression[index] == '*' ||
               expression[index] == '/') {
        thisTreeNode->data = expression[index];
        thisTreeNode->rightNode = new TreeType::Node();
        thisTreeNode->rightNode->parentNode = thisTreeNode;
        insertTN(thisTreeNode->rightNode, expression, index + 1);
    } else if (std::isdigit(expression[index]) || std::isalpha(expression[index])) {
        thisTreeNode->data = expression[index];
        insertTN(thisTreeNode->parentNode, expression, index + 1);
    } else if (expression[index] == ')') {
        insertTN(thisTreeNode->parentNode, expression, index + 1);
    } else if (expression[index] == ' '){
        insertTN(thisTreeNode, expression, index + 1);
    }
}
}

template<typename ElementType>
void TreeType<ElementType>::insert(std::string expression, int index) {
    insertTN(rootNode,expression,index);
}


#endif // !_TREE_H_
