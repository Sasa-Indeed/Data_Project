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

};


#endif //DS_PROJECT_SCANNER_SCANNER_H
