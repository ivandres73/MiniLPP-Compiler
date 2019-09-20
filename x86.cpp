#include "x86.h"

void x86::printCFG(CFGStmt* root) {
    cout << "i'm print cfg\n";
    root->toString();
}