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
    //For inserting in the tree
    void insertTN(NodePointer& thisTreeNode, std::string expression, int index);

    void inorder(NodePointer& thisTreeNode);
    void preorder(NodePointer& thisTreeNode);
    void postorder(NodePointer& thisTreeNode);

public:
    //Default constructor
    TreeType();

    //Destructor
    ~TreeType();

    //Copy constructor
    TreeType(const TreeType<ElementType> & toCopyTree);

    void insert(std::string expression);

    //Is empty method
    bool isEmpty();

    void displayInorder() {
        inorder(rootNode);
    }

    void displayPreorder() {
        preorder(rootNode);
    }

    void displayPostorder() {
        postorder(rootNode);
    }

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

/*template<typename ElementType>
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
        std::string temp;
        for (; std::isdigit(expression[index]) || std::isalpha(expression[index]) ; ++index) {
            temp+= expression[index];
        }
        thisTreeNode->data = temp;
        insertTN(thisTreeNode->parentNode, expression, index );
    } else if (expression[index] == ')') {
        insertTN(thisTreeNode->parentNode, expression, index + 1);
    } else if (expression[index] == ' '){
        insertTN(thisTreeNode, expression, index + 1);
    }
}
}*/



template<typename ElementType>
void TreeType<ElementType>::insert(std::string expression) {
    //currNode & index for traversing
    TreeType::NodePointer currNode = this->rootNode;
    int index = 0;

    while (index < expression.length()){
        /*If the token is a '(' then it will
         * create a left Node and assigning
         * the left node parent to current node
         * then transcending into the left Node */
        if (expression[index] == '('){
            currNode->leftNode = new TreeType::Node();
            currNode->leftNode->parentNode = currNode;
            currNode = currNode->leftNode;
            index++;
        } else if (expression[index] == '+' || expression[index] == '-' || expression[index] == '*' ||
                   expression[index] == '/'){
            /*Assigning the currNode data to the
             * current token and creating a right node
             * making its parent to the current node then
             * transcending to the right node*/
            currNode->data = expression[index++];
            currNode->rightNode = new TreeType::Node();
            currNode->rightNode->parentNode = currNode;
            currNode = currNode->rightNode;
        } else if(std::isdigit(expression[index]) || std::isalpha(expression[index])){
            /*looping to get if it is a multi-able set of
             * characters or numbers*/
            std::string temp;
            for (; std::isdigit(expression[index]) || std::isalpha(expression[index]) ; ++index) {
                temp+= expression[index];
            }
            //Assigning the currNode to the data then returning to the parent Node
            currNode->data = temp;
            currNode = currNode->parentNode;
        } else if (expression[index] == ')'){
            /*If it is the end of the expression
             * then it will return to the parent Node*/
            currNode = currNode->parentNode;
            index++;
        }

    }


   // insertTN(rootNode,expression,0);
}

template<typename ElementType>
void TreeType<ElementType>::inorder(NodePointer& thisTreeNode) {
    if (thisTreeNode != NULL){
        inorder(thisTreeNode->leftNode);
        std::cout<< thisTreeNode->data;
        inorder(thisTreeNode->rightNode);
    }

}

template<typename ElementType>
void TreeType<ElementType>::preorder(TreeType::NodePointer &thisTreeNode) {
    if ( thisTreeNode != NULL){
        std::cout<< thisTreeNode->data ;
        preorder(thisTreeNode->leftNode);
        preorder(thisTreeNode->rightNode);
    }
}

template<typename ElementType>
void TreeType<ElementType>::postorder(TreeType::NodePointer &thisTreeNode) {
    if ( thisTreeNode != NULL){
        postorder(thisTreeNode->leftNode);
        postorder(thisTreeNode->rightNode);
        std::cout<< thisTreeNode->data;
    }
}


#endif // !_TREE_H_
