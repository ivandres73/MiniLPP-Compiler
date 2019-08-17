%{
    #include "exprParser.h"
%}

%token decNum
%token hexNum
%token binNum
%token Char
%token String
%token Iden
%token opAdd '+'
%token opMul '*'
%token opDiv 'div'
%token opSub '-'
%token opMod 'mod'
%token opPow '^'
%token Error,
%token Space ' '
%token opBra '['
%token clBra ']'
%token Comma ','
%token Colon ';'
%token opPar '('
%token clPar ')'
%token Assign '<-'
%token leThan '<'
%token grThan '>'
%token Equal '='
%token leEqu '<='
%token grEqu '>='
%token kwEntero
%token kwReal
%token kwCadena
%token kwBooleano,
%token kwCaracter,
%token kwArreglo,
%token kwDe,
%token kwFuncion,
%token kwProcedimiento,
%token kwVar,
%token kwInicio,
%token kwFin,
%token kwFinal,
%token kwSi,
%token kwEntonces,
%token kwSino,
%token kwPara,
%token kwMientras,
%token kwHaga,
%token kwLlamar,
%token kwRepita,
%token kwHasta,
%token kwCaso,
%token kwO,
%token kwY,
%token kwNo,
%token kwLea,
%token kwEscriba,
%token kwRetorne,
%token kwTipo,
%token kwEs,
%token kwRegistro,
%token kwArchivo,
%token kwSecuencial,
%token kwAbrir,
%token kwComo,
%token kwLectura,
%token kwEscritura,
%token kwCerrar,
%token kwLeer,
%token kwEscribir,
%token kwVerdadero,
%token kwFalso,
%token Eof

%%