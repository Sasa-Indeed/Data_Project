//
// Created by Mohamed Ali on 29-Nov-22.
//

#include <iostream>

#include "ParseTree.h"

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

bool ParseTree::checkPeek(std::string kind) {
    return kind == peekToken;
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
    while(!checkToken("EOF")){
        ParseTree::statement();
    }
    for(auto label:labelsGOTOed){
        if(!labelsDeclared.count(label)){
            abort("Attempting to GOTO to undeclared label: " + label);
        }
    }
}

void ParseTree::statement() {
    if(checkToken("if")){
        std::cout<<"STATEMENT-IF"<<std::endl;
        nextToken();
        comparison();
        match("then");
        while(!checkToken("end")){
            statement();
        }
        match("end");
    }
    else if(checkToken("repeat")){
        std::cout<<"STATEMENT-REPEAT"<<std::endl;
        nextToken();
        while(!checkToken("until")){
            statement();
        }
        match("until");
        comparison();
        semiColon();
    }
    else if(checkToken("read")){
        std::cout<<"STATEMENT-READ"<<std::endl;
        nextToken();
        match("identifier");
        symbols.insert(codeList[curPos-2]);
        std::cout<<codeList[curPos-2]<<std::endl;
        semiColon();
    }
    else if(checkToken("write")){
        std::cout<<"STATEMENT-WRITE"<<std::endl;
        nextToken();
        expression();
        semiColon();
    }
    else if(checkToken("identifier")){
        std::cout<<"STATEMENT-ASSIGN"<<std::endl;
        if(!symbols.count(codeList[curPos-1])){
            symbols.insert(codeList[curPos-1]);
        }
        nextToken();
        match(":=");
        expression();
        semiColon();
    }
}

void ParseTree::comparison() {
    std::cout<<"COMPARISON"<<std::endl;
    expression();
    if(isComparisonOperator()){
        nextToken();
        expression();
    }
    else{
        abort("Expected comparison operator at: " + codeList[curPos]);
    }
    while(isComparisonOperator()){
        nextToken();
        expression();
    }
}

void ParseTree::expression() {
    std::cout<<"EXPRESSION"<<std::endl;
    term();
    while(checkToken("+") || checkToken("-")){
        nextToken();
        term();
    }
}

void ParseTree::term() {
    std::cout<<"TERM"<<std::endl;
    unary();
    while(checkToken("*") || checkToken("/")){
        nextToken();
        unary();
    }
}

void ParseTree::unary() {
    std::cout<<"UNARY"<<std::endl;
    while(checkToken("+") || checkToken("-")){
        nextToken();
    }
    primary();
}

void ParseTree::primary() {
    std::cout<<"PRIMARY("<<codeList[curPos-1]<<")"<<std::endl;
    if(checkToken("number")){
        nextToken();
    }
    else if(checkToken("identifier")){
        if(!symbols.count(codeList[curPos-1])){
            abort("Referencing variable before assignment: " + codeList[curPos-1]);
        }
        nextToken();
    }
    else{
        abort("Unexpected token at " + codeList[curPos]);
    }
}

void ParseTree::semiColon() {
    std::cout<<";"<<std::endl;
    match(";");
}