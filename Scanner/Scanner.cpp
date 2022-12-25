//
// Created by Mohamed Ali on 16-Nov-22.
//

#include "D:\MSVProjects\TINYLangScreens\TINYLangScreens\Scanner.h"
#include <iostream>
#include <string>
#include <sstream>
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

std::string Scanner::translate(std::string str, std::string find, std::string replace) {
    using namespace std;
    string result;
    size_t find_len = find.size();
    size_t pos,from=0;
    while ( string::npos != ( pos=str.find(find,from) ) ) {
        result.append( str, from, pos-from );
        result.append( replace );
        from = pos + find_len;
    }
    result.append( str, from , string::npos );
    return result;
}
Scanner::Scanner(std::string tinyCode) {
    tinyCode = translate(tinyCode, ":", "\\:");
    tinyCode = translate(tinyCode, "-", "\\-");
    tinyCode = translate(tinyCode, "]", "\\]");
    tinyCode = translate(tinyCode, "^", "\\^");
    tinyCode = translate(tinyCode, "$", "\\$");
    tinyCode = translate(tinyCode, "*", "\\*");
    tinyCode = translate(tinyCode, ".", "\\.");
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
    std::string state = "start";
    for(auto tiny_in: tokens){
        std::string token_str = "";
        int i = 0;
        while (i < tiny_in.size()){
            if(inArray(tiny_in[i], special_chars) && state != "inassign" && state != "incomment"){
                if(token_str != ""){
                    tokensList.push_back(token_str);
                    token_str = "";
                }
                char* temp = &tiny_in[i];
                std::string temp1;
                if ((tiny_in[i] == '>' && tiny_in[i+1] == '=') || (tiny_in[i] == '<' && tiny_in[i+1] == '=')){
                    temp1.push_back(temp[0]);
                    temp1.push_back(temp[1]);
                    tokensList.push_back(temp1);
                    state = "start";
                    i+= 1;
                }
                else{temp1.push_back(temp[0]);
                tokensList.push_back(temp1);
                state = "start";}
            }
            else if(state == "start"){
                if(tiny_in[i] == ' '){
                    state = "start";
                }
                else if(isalpha(tiny_in[i])){
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
                    state = "incomment";
                }
                else{
                    state = "done";
                }
            }
            else if(state == "inid"){
                if(std::isalpha(tiny_in[i])){
                    token_str += tiny_in[i];
                }
                else{
                    state = "done";
                }
            }
            else if(state == "isnum"){
                if(std::isdigit(tiny_in[i])){
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
                    state = "start";
                }

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
            tokenOutputs.push_back(token);
        }
        else if(inArray(token, special_chars_string) || token == ">=" || token == "<="){
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
    std::vector<std::string> temp = tokensList;
    codeList = temp;
    tokensList = tokenOutputs;
}

std::vector<std::string> Scanner::getCodeList() {
    return codeList;
}

std::vector<std::string> Scanner::getTokenList() {
    return tokensList;
}