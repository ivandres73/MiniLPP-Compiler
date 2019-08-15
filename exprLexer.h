#include <iostream>
#include <string>
#include <fstream>
#include <cstring>

using std::cout;
using std::endl;
using std::istream;
using std::string;
using std::ostream;

#define SIZE 1024

enum class Token {
    decNum,
    hexNum,
    binNum,
    Char,
    String,
    Iden,
    opAdd,
    opMul,
    opDiv,
    opSub,
    Error,
    Space,
    opBra,
    clBra,
    Comma,
    Colon,
    opPar,
    clPar,
    Assign,
    Pow,
    leThan,
    grThan,
    Equal,
    leEqu,
    grEqu,
    Eof
};

ostream& operator<<(ostream&, Token);

class exprLexer {
public:
    exprLexer(std::ifstream& in) : ctx(in), line(1){};
    Token getNextToken();
    string getText() { return text; }
    
private:
    int line;
    string text;

    enum class fillstatus {
        Error,
        Eof,
        Ok
    };
    struct context {
        char *lim;
        char *curs;
        char *tok;
        char *buf;
        char *mark;
        bool eof;
        std::istream& in;
        fillstatus fill(size_t);

        context(istream& in);
        ~context() {
            delete[] buf;
        }
        string tokenText() {
            return string(tok, curs - tok);
        }
    };
     Token makeToken(Token tk) {
        text = ctx.tokenText();
        return tk;
    }
    context ctx;

};