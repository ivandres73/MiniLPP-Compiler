#include "build/cfg.h"
#include <fstream>

using std::ofstream;

class x86 {
    public:
        string CFGtox86(CFGStmt *root, Context&);
};