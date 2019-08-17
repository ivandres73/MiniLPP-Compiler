#include <iostream>
#include <string>
#include <fstream>
#include <cstring> // for memmove() and memset()
#include "Tokens.h"

using std::cout;
using std::endl;
using std::istream;
using std::string;
using std::ostream;

#define SIZE 1024

ostream& operator<<(ostream&, Token);

class exprLexer {
public:
    exprLexer(std::ifstream& in) : ctx(in), line(1) { }
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
        istream& in;
        fillstatus fill(size_t);

        context(istream&);
        ~context() {
            delete[] buf;
        }
        string tokenText(const Token &tk) {
            if (tk == Token::Char || tk == Token::String) { //omitting the apostrophes in chars
                tok++;
                curs--;
            }
            string str(tok, curs - tok);
            if (tk == Token::Char || tk == Token::String)
                curs++;
            return str;
        }
    };
    Token makeToken(Token tk) {
        text = ctx.tokenText(tk);
        return tk;
    }
    context ctx;

};