#include <iostream>
#include "C:\\Users\\Mohamed Ali\\CLionProjects\\DS_Project_scanner\\Scanner.cpp"
#include "C:\Users\Mohamed Ali\CLionProjects\Parser\cmake-build-debug\ParseTree.h"

int main() {

    Scanner scan("read x;\nif 0<=-x then {sasa}\nfact:=1;\nrepeat\nfact:=fact*x;\nx:=x-1;\nuntil x=0;\nwrite fact;\nend");
    scan.scan();
    ParseTree pt(scan.getTokenList(), scan.getCodeList());
    pt.program();
    pt.~ParseTree();
    return 0;
}
