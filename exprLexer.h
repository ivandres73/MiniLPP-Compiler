#include <iostream>
#include <string>
#include <fstream>
#include <cstring> // for memmove() and memset()
#include "build/Tokens.h"

using std::cout;
using std::endl;
using std::istream;
using std::string;
using std::ostream;

#define SIZE 1024

class exprLexer {
public:
    exprLexer(std::ifstream& in) : ctx(in), line(1) { }
    yytokentype getNextToken();
    string getText() { return text; }
    int getLine() { return line; }
    
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
        string tokenText(const yytokentype &tk) {
            if (tk == yytokentype::Char || tk == yytokentype::String) { //omitting the apostrophes in chars
                tok++;
                curs--;
            }
            if (tk == yytokentype::Eol) {

            }
            string str(tok, curs - tok);
            if (tk == yytokentype::Char || tk == yytokentype::String)
                curs++;
            return str;
        }
    };
    yytokentype makeToken(const yytokentype &tk) {
        text = ctx.tokenText(tk);
        return tk;
    }
    context ctx;

};