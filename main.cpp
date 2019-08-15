#include "exprLexer.h"

using std::cerr;
using std::ifstream;
using std::stringstream;

int main(int argc, char* argv[]) {

    if (argc != 2) {
        cerr << "usage: " << argv[0] << " <input file>\n";
        return 1;
    }
    ifstream file(argv[1]);
    exprLexer lex(file);
    Token tk;
    do {
        tk = lex.getNextToken();
        if (tk != Token::Space)
            cout << '\'' << tk << "\' " << lex.getText() << endl;
    } while(tk != Token::Eof);

    return 0;
}