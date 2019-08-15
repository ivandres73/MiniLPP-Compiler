#include "../exprLexer.h"
/*!max:re2c*/
/*!re2c
    digit  = [0-9];
    number = digit+;
    hex    = [a-fA-F];
    string = .+;
    iden   = [_a-zA-Z];
*/  

Token exprLexer::getNextToken() {

    #define YYFILL(n)\
    do\
    {\
        fillstatus x = ctx.fill(n);\
        switch (x)\
        {\
        case fillstatus::Error:\
            return Token::Error;\
        case fillstatus::Eof:\
            return Token::Eof;\
        }\
    } while(0)\

    while (true) {
        ctx.tok = ctx.curs;
        /*!re2c
            re2c:define:YYCTYPE = char;
            re2c:define:YYLIMIT = ctx.lim;
            re2c:define:YYCURSOR = ctx.curs;
            re2c:define:YYMARKER = ctx.mark;
            re2c:define:YYFILL:naked = 0;

            *      {return makeToken(Token::Error);}
            [ \t]  {continue;} //whitespaces
            "\n"+  {return makeToken(Token::Space);}
            iden(iden|[0-9])+ {return makeToken(Token::Iden);}
            number {return makeToken(Token::decNum);}
            '0b'[0-1]+ {return makeToken(Token::binNum);}
            '0b'number {return makeToken(Token::Error);}
            '0x'(hex|digit)+ {return makeToken(Token::hexNum);}
            '0x'([a-zA-Z]|digit)+ {return makeToken(Token::Error);}
            "'"."'"|"'''" {return makeToken(Token::Char);}
            "\""string"\"" {return makeToken(Token::String);}
            "+"    {return makeToken(Token::opAdd);}
            "*"    {return makeToken(Token::opMul);}
            "-"    {return makeToken(Token::opSub);}
            'div'    {return makeToken(Token::opDiv);}
            "["    {return makeToken(Token::opBra);}
            "]"    {return makeToken(Token::clBra);}
            ","    {return makeToken(Token::Comma);}
            ":"    {return makeToken(Token::Colon);}
            "("    {return makeToken(Token::opPar);}
            ")"    {return makeToken(Token::clPar);}
            "<-"    {return makeToken(Token::Assign);}
            "^"    {return makeToken(Token::Pow);}
            "<"    {return makeToken(Token::leThan);}
            ">"    {return makeToken(Token::grThan);}
            "="    {return makeToken(Token::Equal);}
            "<="    {return makeToken(Token::leEqu);}
            ">="    {return makeToken(Token::grEqu);}
            "\x00" {return makeToken(Token::Eof);}
        */
    }
}

exprLexer::context::context(istream& in): in(in) {
    buf = new char[SIZE + YYMAXFILL];
    lim = buf + SIZE;
    curs = lim;
    tok = lim;
    eof = false;
}

exprLexer::fillstatus exprLexer::context::fill(size_t need) {
    if (eof)
        return fillstatus::Eof;

    const size_t free = tok - buf;

    if (free < need)
        return fillstatus::Error;

    memmove(buf, tok, lim-tok);
    lim -= free;
    curs -= free;
    tok -= free;

    in.read(lim, free);
    lim += in.gcount();
    if (lim < buf + SIZE) {
        eof = true;
        memset(lim, 0, YYMAXFILL);

        lim += YYMAXFILL;
    }
    return fillstatus::Ok;
}

#define case(name)\
    case Token::name:\
    out << #name;\
    break

ostream& operator<<(ostream& out, Token tk) {
    switch (tk) {
    case(decNum);
    case(binNum);
    case(hexNum);
    case(opAdd);
    case(opMul);
    case(opDiv);
    case(opSub);
    case(Eof);
    case(Space);
    case(Char);
    case(String);
    case(Iden);
    default:
        out << "Error";
        break;
    }
}