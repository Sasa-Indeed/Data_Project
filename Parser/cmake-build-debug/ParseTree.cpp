//
// Created by Mohamed Ali on 29-Nov-22.
//

#include <iostream>
#include <stack>
#include <queue>

#include "ParseTree.h"
ParseTree::~ParseTree() {
    for (auto Node: createdNodes) {
        delete Node;
    }
    delete rootNode;
}
void ParseTree::nextToken() {
    if (curPos < codeList.size() - 1) {
        this->curToken = tokensList[curPos];
        this->peekToken = tokensList[curPos + 1];
        curPos++;
    }
    else if (curPos == codeList.size() - 1){
        this->curToken = tokensList[curPos];
        this->peekToken = "EOF";
        curPos++;
    }
    else
    {
        this->curToken = "EOF";
        this->peekToken = "Something random lol";
    }
}

bool ParseTree::checkToken(std::string kind) {
    return kind == curToken;
}

bool ParseTree::match(std::string kind) {
    if(!checkToken(kind)){
        ParseTree::abort("Expected " + kind + ", got " + curToken);
    }
    nextToken();
}

bool ParseTree::isComparisonOperator() {
    return checkToken(">") || checkToken(">=")|| checkToken("<") || checkToken("<=")|| checkToken("=");
}

void ParseTree::abort(std::string msg) {
    std::cout<<"Error. "<<msg<<std::endl;
    exit(1);
}

void ParseTree::program() {
    std::stack<Node*> s;
    while(!checkToken("EOF")){
        ParseTree::statement(rootNode, s);
    }
}

void ParseTree::statement(Node* parent, std::stack<Node*> s) {
    s.push(parent);
    if(checkToken("if")){
//        std::cout<<"STATEMENT-IF"<<std::endl;
        Node* currParent = s.top();
        s.pop();
        std::vector<Node*> currentIf = insertIf(currParent);
        Node* currentIfBody = currentIf[1];
        nextToken();
        comparison(insertComparison(currentIf[0]));
        match("then");
        while(!checkToken("end") && !checkToken("EOF")){
            statement(currentIfBody, s);
        }
        match("end");

    }
    else if(checkToken("repeat")){
//        std::cout<<"STATEMENT-REPEAT"<<std::endl;
        Node* currParent = s.top();
        s.pop();
        std::vector<Node*> currRepeat = insertRepeat(currParent);
        nextToken();
        while(!checkToken("until")){
            statement(currRepeat[1], s);
        }
        match("until");
        comparison(insertComparison(currRepeat[0]));
        semiColon();
    }
    else if(checkToken("read")){
        /*std::cout<<"STATEMENT-READ"<<std::endl;*/
        nextToken();
        match("identifier");
        std::string id = codeList[curPos-2];
        symbols.insert(codeList[curPos-2]);
        std::cout<<codeList[curPos-2]<<std::endl;
        semiColon();
        insertRead(parent, "ID " + id);
    }
    else if(checkToken("write")){
//        std::cout<<"STATEMENT-WRITE"<<std::endl;
        Node* currWrite = new Node("STATEMENT-WRITE");
        createdNodes.push_back(currWrite);
        nextToken();
        expression(insertExpression(currWrite));
        semiColon();
        insertWrite(parent, currWrite);
    }
    else if(checkToken("identifier")){
//        std::cout<<"STATEMENT-ASSIGN"<<std::endl;
        if(!symbols.count(codeList[curPos-1])){
            symbols.insert(codeList[curPos-1]);
        }
        nextToken();
        match(":=");
        expression(insertExpression(insertAssign(parent, "ID " + codeList[curPos - 3])));
        semiColon();
    }
}

void ParseTree::comparison(Node* currComparison) {
//    std::cout<<"COMPARISON"<<std::endl;
    expression(insertExpression(currComparison));
    if(isComparisonOperator()){
        nextToken();
        expression(insertExpression(currComparison));
    }
    else{
        abort("Expected comparison operator at: " + codeList[curPos]);
    }
    while(isComparisonOperator()){
        nextToken();
        expression(insertExpression(currComparison));
    }
}

ParseTree::Node* ParseTree::expression(Node* currExpression) {
//    std::cout<<"EXPRESSION"<<std::endl;
    term(insertTerm(currExpression));
    while(checkToken("+") || checkToken("-")){
        nextToken();
        term(insertTerm(currExpression));
    }
}

void ParseTree::term(Node* currTerm) {
//    std::cout<<"TERM"<<std::endl;
    if(codeList[curPos - 1] == "-")
    unary(insertUnary(currTerm, codeList[curPos - 1]));
    else{
        unary(insertUnary(currTerm, "+"));
    }
    while(checkToken("*") || checkToken("/")){
        nextToken();
        if(codeList[curPos - 1] == "-")
        unary(insertUnary(currTerm, codeList[curPos - 1]));
        else{
            unary(insertUnary(currTerm, "+"));
        }
    }
}

std::string ParseTree::unary(Node* currUnary) {
//    std::cout<<"UNARY"<<std::endl;
    while(checkToken("+") || checkToken("-")){
        nextToken();
    }
    int tempCurPos = curPos - 2;
    insertPrimary(currUnary, primary(currUnary));
    return codeList[tempCurPos];
}

std::string ParseTree::primary(Node* parent) {
//    std::cout<<"PRIMARY("<<codeList[curPos-1]<<")"<<std::endl;

    if(checkToken("number")){
        nextToken();
        return "NUMBER " + codeList[curPos - 2];
    }
    else if(checkToken("identifier")){
        if(!symbols.count(codeList[curPos-1])){
            abort("Referencing variable before assignment: " + codeList[curPos-1]);
        }
        nextToken();
        return "ID " + codeList[curPos - 2];
    }
    else{
        abort("Unexpected token at " + codeList[curPos]);
    }
}

void ParseTree::semiColon() {
//    std::cout<<";"<<std::endl;
    match(";");
}

void ParseTree::insertRead(ParseTree::Node *parent, std::string id) {
    Node* readSubTree = new Node("STATEMENT-READ");
    createdNodes.push_back(readSubTree);
    readSubTree->Children.push_back(new Node(id));
    parent->Children.push_back(readSubTree);
}

void ParseTree::insertPrimary(ParseTree::Node *parent, std::string value) {
    Node* primarySubTree = new Node("PRIMARY");
    createdNodes.push_back(primarySubTree);
    primarySubTree->Children.push_back(new Node(value));
    parent->Children.push_back(primarySubTree);
}

ParseTree::Node* ParseTree::insertUnary(ParseTree::Node *parent, std::string value) {
    Node* unarySubTree = new Node("UNARY " + value);
    createdNodes.push_back(unarySubTree);
    parent->Children.push_back(unarySubTree);
    return unarySubTree;
}
ParseTree::Node* ParseTree::insertTerm(ParseTree::Node *parent) {
    Node* termSubTree = new Node("TERM");
    createdNodes.push_back(termSubTree);
    parent->Children.push_back(termSubTree);
    return termSubTree;
}

ParseTree::Node* ParseTree::insertExpression(ParseTree::Node *parent) {
    Node* expressionSubTree = new Node("EXPRESSION");
    createdNodes.push_back(expressionSubTree);
    parent->Children.push_back(expressionSubTree);
    return expressionSubTree;
}

ParseTree::Node* ParseTree::insertAssign(ParseTree::Node *parent, std::string value) {
    Node* assignSubTree = new Node("STATEMENT-ASSIGN");
    createdNodes.push_back(assignSubTree);
    assignSubTree->Children.push_back(new Node(value));
    parent->Children.push_back(assignSubTree);
    return assignSubTree;
}
ParseTree::Node* ParseTree::insertComparison(ParseTree::Node *parent) {
    Node* comparisonSubTree = new Node("COMPARISON");
    createdNodes.push_back(comparisonSubTree);
    parent->Children.push_back(comparisonSubTree);
    return comparisonSubTree;
}

void ParseTree::insertWrite(ParseTree::Node *parent, ParseTree::Node* self) {
    parent->Children.push_back(self);
}

std::vector<ParseTree::Node*> ParseTree::insertRepeat(ParseTree::Node *parent) {
    std::vector<Node*> repeatSubTree;
    repeatSubTree.push_back(new Node("STATEMENT-REPEAT"));
    repeatSubTree[0]->Children.push_back(new Node("Body"));
    repeatSubTree.push_back(repeatSubTree[0]->Children[0]);
    parent->Children.push_back(repeatSubTree[0]);
    createdNodes.push_back(repeatSubTree[0]);
    createdNodes.push_back(repeatSubTree[1]);
    return repeatSubTree;
}

std::vector<ParseTree::Node*> ParseTree::insertIf(ParseTree::Node *parent) {
    std::vector<Node*> ifSubTree;
    ifSubTree.push_back(new Node("STATEMENT-IF"));
    ifSubTree[0]->Children.push_back(new Node("BODY"));
    Node* Body = ifSubTree[0]->Children[0];
    ifSubTree.push_back(Body);
    parent->Children.push_back(ifSubTree[0]);
    createdNodes.push_back(ifSubTree[0]);
    createdNodes.push_back(ifSubTree[1]);
    return ifSubTree;
}