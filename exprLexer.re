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

            *                     {return makeToken(Token::Error);}
            [ \t]                 {continue;} //whitespaces
            "\n"+                 {return makeToken(Token::Space);}
            number                {return makeToken(Token::decNum);}
            '0b'[0-1]+            {return makeToken(Token::binNum);}
            '0b'number            {return makeToken(Token::Error);}
            '0x'(hex|digit)+      {return makeToken(Token::hexNum);}
            '0x'([a-zA-Z]|digit)+ {return makeToken(Token::Error);}
            "'"."'"|"'''"         {return makeToken(Token::Char);}
            "\""string"\""        {return makeToken(Token::String);}
            "+"                   {return makeToken(Token::opAdd);}
            "*"                   {return makeToken(Token::opMul);}
            "-"                   {return makeToken(Token::opSub);}
            "^"                   {return makeToken(Token::opPow);}
            'div'                 {return makeToken(Token::opDiv);}
            'mod'                 {return makeToken(Token::opMod);}
            "["                   {return makeToken(Token::opBra);}
            "]"                   {return makeToken(Token::clBra);}
            ","                   {return makeToken(Token::Comma);}
            ":"                   {return makeToken(Token::Colon);}
            "("                   {return makeToken(Token::opPar);}
            ")"                   {return makeToken(Token::clPar);}
            "<-"                  {return makeToken(Token::Assign);}
            "<"                   {return makeToken(Token::leThan);}
            ">"                   {return makeToken(Token::grThan);}
            "="                   {return makeToken(Token::Equal);}
            "<="                  {return makeToken(Token::leEqu);}
            ">="                  {return makeToken(Token::grEqu);}
            'Entero'              {return makeToken(Token::kwEntero);}
            'Real'                {return makeToken(Token::kwReal);}
            'Cadena'              {return makeToken(Token::kwCadena);}
            'Booleano'            {return makeToken(Token::kwBooleano);}
            'Caracter'            {return makeToken(Token::kwCaracter);}
            'Arreglo'             {return makeToken(Token::kwArreglo);}
            'De'                  {return makeToken(Token::kwDe);}
            'Funcion'             {return makeToken(Token::kwFuncion);}
            'Procedimiento'       {return makeToken(Token::kwProcedimiento);}
            'Var'                 {return makeToken(Token::kwVar);}
            'Inicio'              {return makeToken(Token::kwInicio);}
            'Fin'                 {return makeToken(Token::kwFin);}
            'Final'               {return makeToken(Token::kwFinal);}
            'Si'                  {return makeToken(Token::kwSi);}
            'Entonces'            {return makeToken(Token::kwEntonces);}
            'Sino'                {return makeToken(Token::kwSino);}
            'Para'                {return makeToken(Token::kwPara);}
            'Mientras'            {return makeToken(Token::kwMientras);}
            'Haga'                {return makeToken(Token::kwHaga);}
            'Llamar'              {return makeToken(Token::kwLlamar);}
            'Repita'              {return makeToken(Token::kwRepita);}
            'Hasta'               {return makeToken(Token::kwHasta);}
            'Caso'                {return makeToken(Token::kwCaso);}
            'O'                   {return makeToken(Token::kwO);}
            'Y'                   {return makeToken(Token::kwY);}
            'No'                  {return makeToken(Token::kwNo);}
            'Lea'                 {return makeToken(Token::kwLea);}
            'Escriba'             {return makeToken(Token::kwEscriba);}
            'Retorne'             {return makeToken(Token::kwRetorne);}
            'Tipo'                {return makeToken(Token::kwTipo);}
            'Es'                  {return makeToken(Token::kwEs);}
            'Registro'            {return makeToken(Token::kwRegistro);}
            'Archivo'             {return makeToken(Token::kwArchivo);}
            'Secuencial'          {return makeToken(Token::kwSecuencial);}
            'Abrir'               {return makeToken(Token::kwAbrir);}
            'Como'                {return makeToken(Token::kwComo);}
            'Lectura'             {return makeToken(Token::kwLectura);}
            'Escritura'           {return makeToken(Token::kwEscritura);}
            'Cerrar'              {return makeToken(Token::kwCerrar);}
            'Leer'                {return makeToken(Token::kwLeer);}
            'Escribir'            {return makeToken(Token::kwEscribir);}
            'Verdadero'           {return makeToken(Token::kwVerdadero);}
            'Falso'               {return makeToken(Token::kwFalso);}
            iden(iden|[0-9])+     {return makeToken(Token::Iden);}
            "\x00"                {return makeToken(Token::Eof);}
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
    case(opMod);
    case(opPow);
    case(Eof);
    case(Space);
    case(Char);
    case(String);
    case(Iden);
    case(kwEntero);
    case(kwReal);
    case(kwCadena);
    case(kwBooleano);
    case(kwCaracter);
    case(kwArreglo);
    case(kwDe);
    case(kwFuncion);
    case(kwProcedimiento);
    case(kwVar);
    case(kwInicio);
    case(kwFin);
    case(kwFinal);
    case(kwSi);
    case(kwEntonces);
    case(kwSino);
    case(kwPara);
    case(kwMientras);
    case(kwHaga);
    case(kwLlamar);
    case(kwRepita);
    case(kwHasta);
    case(kwCaso);
    case(kwO);
    case(kwY);
    case(kwNo);
    case(kwLea);
    case(kwEscriba);
    case(kwRetorne);
    case(kwTipo);
    case(kwEs);
    case(kwRegistro);
    case(kwArchivo);
    case(kwSecuencial);
    case(kwAbrir);
    case(kwComo);
    case(kwLectura);
    case(kwEscritura);
    case(kwCerrar);
    case(kwLeer);
    case(kwEscribir);
    case(kwVerdadero);
    case(kwFalso);
    default:
        out << "Error";
        break;
    }
}