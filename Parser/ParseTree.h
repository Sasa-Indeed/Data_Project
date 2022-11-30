//
// Created by Mohamed Ali on 29-Nov-22.
//

#ifndef PARSER_PARSETREE_H
#define PARSER_PARSETREE_H


#include <vector>
#include <string>
#include <set>

class ParseTree {
    private:
        int curPos;
        std::vector<std::string> tokensList;
        std::vector<std::string> codeList;
        std::set<std::string> symbols;
        std::set<std::string> labelsDeclared;
        std::set<std::string> labelsGOTOed;
        std::string curToken;
        std::string peekToken;
        class Node{
            public:
                std::string Data;
                std::vector<Node*> Children;
                Node();
                Node(std::string value){
                    Data = value;
                }
        };
        Node* rootNode;
    public:
        ParseTree(std::vector<std::string> tokens, std::vector<std::string> code){
            rootNode = new Node((std::string) "PROGRAM");
            this->tokensList = tokens;
            this->codeList = code;
            curPos = 0;
            nextToken();
        }
      //  ~ParseTree();
        void nextToken();
        bool checkToken(std::string kind);
        bool checkPeek(std::string kind);
        bool match(std::string kind);
        bool isComparisonOperator();
        void abort(std::string msg);
        void program();
        void statement();
        void comparison();
        void expression();
        void term();
        void unary();
        void primary();
        void semiColon();
        void insert(Node* newStatement);
};


#endif //PARSER_PARSETREE_H
