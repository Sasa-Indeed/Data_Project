#include <iostream>
#include "Tree.h"
#include <string>

int main() {
    TreeType<std::string> tr;
    tr.insert("((aa+69)*(c/(d-1896)))",0);
    tr.displayInorder();
    std::cout<<'\n';
    tr.displayPostorder();
    std::cout<<'\n';
    tr.displayPreorder();
    return 0;
}
