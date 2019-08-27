#ifndef _EXPRPARSER_H
#define _EXPRPARSER_H

#include "exprLexer.h"

using std::stoi;

extern int yyparse();

class exprParser {
public:
    exprParser(std::ifstream& in, string name) : lex(in),
        filename(std::move(name)){ }
    int parse() { return yyparse(); };
    exprLexer lex;
private:
    string filename;
    void yyerror(const char*);
};

#endif