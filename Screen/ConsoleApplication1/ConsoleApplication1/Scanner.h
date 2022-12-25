#pragma once
//
// Created by Mohamed Ali on 16-Nov-22.
//
#include <string>
#include <vector>

#ifndef DS_PROJECT_SCANNER_SCANNER_H
#define DS_PROJECT_SCANNER_SCANNER_H


class Scanner {
private:
    std::string tinyCode;
    std::vector<std::string> tokensList;
    std::vector<std::string> codeList;
public:
    Scanner(std::string tinyCode);
    void setTinyCode(std::string tinyCode);
    void scan();
    bool inArray(const char& value, const std::vector<char>& array);
    bool inArray(const std::string& value, const std::vector<std::string>& array);
    bool isNumber(const std::string input);
    bool isAlpha(const std::string input);
    std::vector<std::string> getCodeList();
    std::vector<std::string> getTokenList();
    std::string translate(std::string orgStr, std::string remove, std::string replace);

};


#endif //DS_PROJECT_SCANNER_SCANNER_H

