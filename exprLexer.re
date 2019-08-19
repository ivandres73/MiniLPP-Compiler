#include "../exprLexer.h"
/*!max:re2c*/
/*!re2c
    digit  = [0-9];
    number = digit+;
    hex    = [a-fA-F];
    string = .+;
    iden   = [_a-zA-Z];
    spec   = [a-zA-Z_-")""(""*"&"^"%$#@!`~number \t\n];//for block comments
*/  

yytokentype exprLexer::getNextToken() {

    #define YYFILL(n)\
    do\
    {\
        fillstatus x = ctx.fill(n);\
        switch (x)\
        {\
        case fillstatus::Error:\
            return Error;\
        case fillstatus::Eof:\
            return Eof;\
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

            *                     {return makeToken(Error);}
            [ \t]                 {continue;} //whitespaces
            "//"[^\n]*            {continue;} //line comments
            "/*"spec*"*""/"       {cout << "shie men\n"; continue;} //block comments
            "\n"[ \t\n]*          {return makeToken(Eol);}
            number                {return makeToken(Num);}
            '0b'[0-1]+            {return makeToken(Num);}
            '0b'number            {return makeToken(Error);}
            '0x'(hex|digit)+      {return makeToken(Num);}
            '0x'([a-zA-Z]|digit)+ {return makeToken(Error);}
            "'"."'"|"'''"         {return makeToken(Char);}
            "\""string"\""        {return makeToken(String);}
            "+"                   {return makeToken(opAdd);}
            "*"                   {return makeToken(opMul);}
            "-"                   {return makeToken(opSub);}
            "^"                   {return makeToken(opPow);}
            'div'                 {return makeToken(opDiv);}
            'mod'                 {return makeToken(opMod);}
            "["                   {return makeToken(opBra);}
            "]"                   {return makeToken(clBra);}
            ","                   {return makeToken(Comma);}
            ":"                   {return makeToken(Colon);}
            "("                   {return makeToken(opPar);}
            ")"                   {return makeToken(clPar);}
            "<-"                  {return makeToken(Assign);}
            "<"                   {return makeToken(leThan);}
            ">"                   {return makeToken(grThan);}
            "="                   {return makeToken(Equal);}
            "<>"                  {return makeToken(NotEq);}
            "<="                  {return makeToken(leEqu);}
            ">="                  {return makeToken(grEqu);}
            'Entero'              {return makeToken(kwEntero);}
            'Real'                {return makeToken(kwReal);}
            'Cadena'              {return makeToken(kwCadena);}
            'Booleano'            {return makeToken(kwBooleano);}
            'Caracter'            {return makeToken(kwCaracter);}
            'Arreglo'             {return makeToken(kwArreglo);}
            'De'                  {return makeToken(kwDe);}
            'Funcion'             {return makeToken(kwFuncion);}
            'Procedimiento'       {return makeToken(kwProcedimiento);}
            'Var'                 {return makeToken(kwVar);}
            'Inicio'              {return makeToken(kwInicio);}
            'Fin'                 {return makeToken(kwFin);}
            'Final'               {return makeToken(kwFinal);}
            'Si'                  {return makeToken(kwSi);}
            'Entonces'            {return makeToken(kwEntonces);}
            'Sino'                {return makeToken(kwSino);}
            'Para'                {return makeToken(kwPara);}
            'Mientras'            {return makeToken(kwMientras);}
            'Haga'                {return makeToken(kwHaga);}
            'Llamar'              {return makeToken(kwLlamar);}
            'Repita'              {return makeToken(kwRepita);}
            'Hasta'               {return makeToken(kwHasta);}
            'Caso'                {return makeToken(kwCaso);}
            'O'                   {return makeToken(kwO);}
            'Y'                   {return makeToken(kwY);}
            'No'                  {return makeToken(kwNo);}
            'Lea'                 {return makeToken(kwLea);}
            'Escriba'             {return makeToken(kwEscriba);}
            'Retorne'             {return makeToken(kwRetorne);}
            'Tipo'                {return makeToken(kwTipo);}
            'Es'                  {return makeToken(kwEs);}
            'Registro'            {return makeToken(kwRegistro);}
            'Archivo'             {return makeToken(kwArchivo);}
            'Secuencial'          {return makeToken(kwSecuencial);}
            'Abrir'               {return makeToken(kwAbrir);}
            'Como'                {return makeToken(kwComo);}
            'Lectura'             {return makeToken(kwLectura);}
            'Escritura'           {return makeToken(kwEscritura);}
            'Cerrar'              {return makeToken(kwCerrar);}
            'Leer'                {return makeToken(kwLeer);}
            'Escribir'            {return makeToken(kwEscribir);}
            'Verdadero'           {return makeToken(kwVerdadero);}
            'Falso'               {return makeToken(kwFalso);}
            iden(iden|digit)*     {return makeToken(Iden);}
            "/x00"                {return Eof;}
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
