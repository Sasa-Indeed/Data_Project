#include <iostream>
#include "Tree.h"
#include <string>

int main() {
    TreeType<std::string> tr;
    tr.insert("((1 + 2) * ( 3 / ( 4 - 5)))",0);
    return 0;
}
