//
// Created by Mohamed Ali on 16-Nov-22.
//

#include "Scanner.h"
#include <bits/stdc++.h>
bool inArray(const char &value, const std::vector<char> &array)
{
    return std::find(array.begin(), array.end(), value) != array.end();
}
Scanner::Scanner(std::string tinyCode) {
    this->tinyCode = tinyCode;
}

void Scanner::scan() {
    std::vector<std::string> tokensList;
    std::vector<std::string> tokens;
    std::string intermediate;
    std::stringstream check1(this->tinyCode);
    while(std::getline(check1, intermediate)){
        tokens.push_back(intermediate);
    }
    for(auto tiny_in: tokens){
        std::string token_str = "";
        std::vector<char> special_chars = {'(', ')','+','-','*','/','=', ';', '<', '>'};
        std::vector<std::string> reserved_words = {"if", "then", "else", "end", "repeat", "until", "read", "write"};
        std::string state = "start";
        int i = 0;
        while (i < tiny_in.size()){
            if(inArray(tiny_in[i], special_chars) && state != "inassign" && state != "incomment"){

            }
        }
    }
}