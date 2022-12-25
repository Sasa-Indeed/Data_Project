#include <iostream>
#include "Scanner.h"
#include "ParseTree.h"

int main() {
    Scanner scan("read x\nif 0<=-x then {sasa}\nfact:=1;\nrepeat\nfact:=fact*x;\nx:=x-1;\nuntil x=0;\nwrite fact;\nend");
    scan.scan();
    ParseTree pt(scan.getTokenList(), scan.getCodeList(), scan.getTokensPerLine());
    pt.program();
    return 0;
}

