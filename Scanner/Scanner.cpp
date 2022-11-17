//
// Created by Mohamed Ali on 16-Nov-22.
//

#include "Scanner.h"
#include <bits/stdc++.h>
bool Scanner::inArray(const char &value, const std::vector<char> &array)
{
    return std::find(array.begin(), array.end(), value) != array.end();
}
bool Scanner::inArray(const std::string &value, const std::vector<std::string> &array)
{
    return std::find(array.begin(), array.end(), value) != array.end();
}
bool Scanner::isNumber(const std::string input){
    for (auto element:input) {
        if(isdigit(element))continue;
        else{return 0;}
    }
    return 1;
}
bool Scanner::isAlpha(const std::string input){
    for(auto element: input){
        if(isalnum(element))continue;
        else{return 0;}
    }
    return 1;
}
Scanner::Scanner(std::string tinyCode) {
    this->tinyCode = tinyCode;
}

void Scanner::scan() {
    //std::vector<std::string> tokensList;
    std::vector<std::string> tokens;
    std::string intermediate;
    std::stringstream check1(this->tinyCode);
    while(std::getline(check1, intermediate)){
        tokens.push_back(intermediate);
    }
    std::vector<char> special_chars = {'(', ')','+','-','*','/','=', ';', '<', '>'};
    std::vector<std::string> special_chars_string = {"(", ")","+","-","*","/","=", ";", "<", ">"};
    std::vector<std::string> reserved_words = {"if", "then", "else", "end", "repeat", "until", "read", "write"};
    for(auto tiny_in: tokens){
        std::string token_str = "";
        std::string state = "start";
        int i = 0;
        while (i < tiny_in.size()){
            if(inArray(tiny_in[i], special_chars) && state != "inassign" && state != "incomment"){
                if(token_str != ""){
                    tokensList.push_back(token_str);
                    token_str = "";
                }
                char* ptr = &tiny_in[i];
                std::string temp = ptr;
                tokensList.push_back(temp);
                state = "start";
            }
            else if(state == "start"){
                if(tiny_in[i] == ' '){
                    state = "start";
                }
                else if(isalnum(tiny_in[i])){
                    token_str += tiny_in[i];
                    state = "inid";
                }
                else if(isdigit(tiny_in[i])){
                    token_str += tiny_in[i];
                    state = "innum";
                }
                else if(tiny_in[i] == ':'){
                    token_str += tiny_in[i];
                    state = "inassign";
                }
                else if(tiny_in[i] == '{'){
                    token_str += tiny_in[i];
                    state = "incomment";
                }
                else{
                    state = "done";
                }
            }
            else if(state == "inid"){
                if((tiny_in[i] >= 'a' && tiny_in[i] <= 'z') || (tiny_in[i] >= 'A' && tiny_in[i] <= 'Z')){
                    token_str += tiny_in[i];
                }
                else{
                    state = "done";
                }
            }
            else if(state == "isnum"){
                if((tiny_in[i] >= '0' and tiny_in[i] <= '9')){
                    token_str += tiny_in[i];
                }
                else{
                    state = "done";
                }
            }
            else if(state == "inassign"){
                if(tiny_in[i] == '='){
                    token_str += tiny_in[i];
                    state = "done";
                }
                else{
                    state = "done";
                }
            }
            else if(state == "incomment"){
                if(tiny_in[i] == '}'){
                    token_str += tiny_in[i];
                    state = "start";
                }
                else{
                    token_str += tiny_in[i];
                }
            }
            else if ((tiny_in[i] == '>' and tiny_in[i+1] == '=') or (tiny_in[i] == '<' and tiny_in[i+1] == '=')){
                token_str = tiny_in[i] + tiny_in[i+1];
                state = "done";
            }
            else if(state == "done"){
                tokensList.push_back(token_str);
                token_str = "";
                state = "start";
                i -= 1;
            }
            i += 1;
        }
        if(token_str != ""){
            tokensList.push_back(token_str);
            token_str = "";
        }
    }
    std::vector<std::string> tokenOutputs;
    for(auto token:tokensList){
        if(inArray(token, reserved_words)){
            tokenOutputs.push_back("reserved word");
        }
        else if(inArray(token, special_chars_string) or token == ">=" or token == "<="){
            tokenOutputs.push_back(token);
        }
        else if(token == ":="){
            tokenOutputs.push_back(token);
        }
        else if(isNumber(token)){
            tokenOutputs.push_back("number");
        }
        else if(isAlpha(token)){
            tokenOutputs.push_back("identifier");
        }
        else{ continue;}
    }
}