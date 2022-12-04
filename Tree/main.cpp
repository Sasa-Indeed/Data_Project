#include <iostream>
#include "Tree.h"
#include <string>
#include <algorithm>
#include <cctype>

std::string deleteSpaces(std::string expression);

int main() {
    TreeType<std::string> tr;


    tr.insert("((a+b)*(c/(d-e)))");
    tr.displayInorder();
    std::cout<<'\n';
    tr.displayPostorder();
    std::cout<<'\n';
    tr.displayPreorder();
    return 0;
}

std::string deleteSpaces(std::string expression) {
    expression.erase(std::remove_if(expression.begin(), expression.end(), ::isspace),
            expression.end());

    return expression;
}
