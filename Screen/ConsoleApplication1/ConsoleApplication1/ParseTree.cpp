//
// Created by Mohamed Ali on 29-Nov-22.
//


#include "ParseTree.h"

ParseTree::~ParseTree() {
    for (auto Node : createdNodes) {
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
    else if (curPos == codeList.size() - 1) {
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

bool ParseTree::match(std::string kind, int line) {
    if (!checkToken(kind)) {
         
        ParseTree::errorDetection("Expected " + kind + "at line " + std::to_string(line) + ", got " + curToken);
        return false;
    }
    else {
        nextToken();
        return true;
    }
}

bool ParseTree::isComparisonOperator() {
    return checkToken(">") || checkToken(">=") || checkToken("<") || checkToken("<=") || checkToken("=");
}

void ParseTree::errorDetection(std::string msg) {
    this->errors.push_back(msg);
}

void ParseTree::program() {
    std::stack<Node*> s;
    int line = 1;
    while (!checkToken("EOF")) {
        ParseTree::statement(rootNode, s,line++);
    }
}

void ParseTree::statement(Node* parent, std::stack<Node*> s,int line) {
    s.push(parent);
    if (checkToken("if")) {
        //        std::cout<<"STATEMENT-IF"<<std::endl;
        Node* currParent = s.top();
        s.pop();
        std::vector<Node*> currentIf = insertIf(currParent);
        Node* currentIfBody = currentIf[1];
        nextToken();
        comparison(insertComparison(currentIf[0]), line);
        if (match("then", line++)) {
            while (!checkToken("end") && !checkToken("EOF")) {
                statement(currentIfBody, s, line++);
            }
            match("end", line);
        }
    }
    else if (checkToken("repeat")) {
        //        std::cout<<"STATEMENT-REPEAT"<<std::endl;
        Node* currParent = s.top();
        s.pop();
        std::vector<Node*> currRepeat = insertRepeat(currParent);
        nextToken();
        while (!checkToken("until")) {
            statement(currRepeat[1], s, line++);
        }
        if (match("until", line)) {
            comparison(insertComparison(currRepeat[0]), line);
            semiColon(line);
        }
    }
    else if (checkToken("read")) {
        /*std::cout<<"STATEMENT-READ"<<std::endl;*/
        nextToken();
        if (match("identifier", line)) {
            std::string id = codeList[curPos - 2];
            symbols.insert(codeList[curPos - 2]);
            semiColon(line);
            insertRead(parent, "ID " + id);
        }
    }
    else if (checkToken("write")) {
        //        std::cout<<"STATEMENT-WRITE"<<std::endl;
        Node* currWrite = new Node("STATEMENT-WRITE");
        createdNodes.push_back(currWrite);
        nextToken();
        expression(insertExpression(currWrite), line);
        semiColon(line);
        insertWrite(parent, currWrite);
    }
    else if (checkToken("identifier")) {
        //        std::cout<<"STATEMENT-ASSIGN"<<std::endl;
        if (!symbols.count(codeList[curPos - 1])) {
            symbols.insert(codeList[curPos - 1]);
        }
        nextToken();
        if (match(":=",line)) {
            expression(insertExpression(insertAssign(parent, "ID " + codeList[curPos - 3])), line);
            semiColon(line);
        }
    }else{
        nextToken();
    }
}

void ParseTree::comparison(Node* currComparison, int line) {
    //    std::cout<<"COMPARISON"<<std::endl;
    expression(insertExpression(currComparison), line);
    if (isComparisonOperator()) {
        nextToken();
        expression(insertExpression(currComparison), line);
    }
    else {
        errorDetection("Expected comparison operator at " + codeList[curPos] + "at line " + std::to_string(line));
    }
    while (isComparisonOperator()) {
        nextToken();
        expression(insertExpression(currComparison), line);
    }
}

void ParseTree::expression(Node* currExpression, int line) {
    //    std::cout<<"EXPRESSION"<<std::endl;
    term(insertTerm(currExpression), line);
    while (checkToken("+") || checkToken("-")) {
        nextToken();
        term(insertTerm(currExpression), line);
    }
}

void ParseTree::term(Node* currTerm, int line) {
    //    std::cout<<"TERM"<<std::endl;
    if (codeList[curPos - 1] == "-")
        unary(insertUnary(currTerm, codeList[curPos - 1]), line);
    else {
        unary(insertUnary(currTerm, "+"), line);
    }
    while (checkToken("*") || checkToken("/")) {
        nextToken();
        if (codeList[curPos - 1] == "-")
            unary(insertUnary(currTerm, codeList[curPos - 1]), line);
        else {
            unary(insertUnary(currTerm, "+"), line);
        }
    }
}

std::string ParseTree::unary(Node* currUnary, int line) {
    //    std::cout<<"UNARY"<<std::endl;
    while (checkToken("+") || checkToken("-")) {
        nextToken();
    }
    int tempCurPos = curPos - 2;
    insertPrimary(currUnary, primary(currUnary, line));
    return codeList[tempCurPos];
}

std::string ParseTree::primary(Node* parent , int line) {
    //    std::cout<<"PRIMARY("<<codeList[curPos-1]<<")"<<std::endl;

    if (checkToken("number")) {
        nextToken();
        return "NUMBER " + codeList[curPos - 2];
    }
    else if (checkToken("identifier")) {
        if (!symbols.count(codeList[curPos - 1])) {
            errorDetection("Referencing variable before assignment: " + codeList[curPos - 1] + "at line " + std::to_string(line));
            return "";
        }
        nextToken();
        return "ID " + codeList[curPos - 2];
    }
    else {
        errorDetection("Unexpected token at " + codeList[curPos] + "at line " + std::to_string(line));
        return "";
    }
}

void ParseTree::semiColon(int line) {
    //    std::cout<<";"<<std::endl;
    match(";" ,line);
}

void ParseTree::insertRead(ParseTree::Node* parent, std::string id) {
    Node* readSubTree = new Node("STATEMENT-READ");
    createdNodes.push_back(readSubTree);
    readSubTree->Children.push_back(new Node(id));
    parent->Children.push_back(readSubTree);
}

void ParseTree::insertPrimary(ParseTree::Node* parent, std::string value) {
    Node* primarySubTree = new Node("PRIMARY");
    createdNodes.push_back(primarySubTree);
    primarySubTree->Children.push_back(new Node(value));
    parent->Children.push_back(primarySubTree);
}

ParseTree::Node* ParseTree::insertUnary(ParseTree::Node* parent, std::string value) {
    Node* unarySubTree = new Node("UNARY " + value);
    createdNodes.push_back(unarySubTree);
    parent->Children.push_back(unarySubTree);
    return unarySubTree;
}
ParseTree::Node* ParseTree::insertTerm(ParseTree::Node* parent) {
    Node* termSubTree = new Node("TERM");
    createdNodes.push_back(termSubTree);
    parent->Children.push_back(termSubTree);
    return termSubTree;
}

ParseTree::Node* ParseTree::insertExpression(ParseTree::Node* parent) {
    Node* expressionSubTree = new Node("EXPRESSION");
    createdNodes.push_back(expressionSubTree);
    parent->Children.push_back(expressionSubTree);
    return expressionSubTree;
}

ParseTree::Node* ParseTree::insertAssign(ParseTree::Node* parent, std::string value) {
    Node* assignSubTree = new Node("STATEMENT-ASSIGN");
    createdNodes.push_back(assignSubTree);
    assignSubTree->Children.push_back(new Node(value));
    parent->Children.push_back(assignSubTree);
    return assignSubTree;
}
ParseTree::Node* ParseTree::insertComparison(ParseTree::Node* parent) {
    Node* comparisonSubTree = new Node("COMPARISON");
    createdNodes.push_back(comparisonSubTree);
    parent->Children.push_back(comparisonSubTree);
    return comparisonSubTree;
}

void ParseTree::insertWrite(ParseTree::Node* parent, ParseTree::Node* self) {
    parent->Children.push_back(self);
}

std::vector<ParseTree::Node*> ParseTree::insertRepeat(ParseTree::Node* parent) {
    std::vector<Node*> repeatSubTree;
    repeatSubTree.push_back(new Node("STATEMENT-REPEAT"));
    repeatSubTree[0]->Children.push_back(new Node("Body"));
    repeatSubTree.push_back(repeatSubTree[0]->Children[0]);
    parent->Children.push_back(repeatSubTree[0]);
    createdNodes.push_back(repeatSubTree[0]);
    createdNodes.push_back(repeatSubTree[1]);
    return repeatSubTree;
}

std::vector<ParseTree::Node*> ParseTree::insertIf(ParseTree::Node* parent) {
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