#ifndef _TOKENS_H
#define _TOKENS_H

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
    opMod,
    opPow,
    Error,
    Space,
    opBra,
    clBra,
    Comma,
    Colon,
    opPar,
    clPar,
    Assign,
    leThan,
    grThan,
    Equal,
    leEqu,
    grEqu,
    kwEntero,//keywords
    kwReal,
    kwCadena,
    kwBooleano,
    kwCaracter,
    kwArreglo,
    kwDe,
    kwFuncion,
    kwProcedimiento,
    kwVar,
    kwInicio,
    kwFin,
    kwFinal,
    kwSi,
    kwEntonces,
    kwSino,
    kwPara,
    kwMientras,
    kwHaga,
    kwLlamar,
    kwRepita,
    kwHasta,
    kwCaso,
    kwO,
    kwY,
    kwNo,
    kwLea,
    kwEscriba,
    kwRetorne,
    kwTipo,
    kwEs,
    kwRegistro,
    kwArchivo,
    kwSecuencial,
    kwAbrir,
    kwComo,
    kwLectura,
    kwEscritura,
    kwCerrar,
    kwLeer,
    kwEscribir,
    kwVerdadero,
    kwFalso,
    Eof
};

#endif