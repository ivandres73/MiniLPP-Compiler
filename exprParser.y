%{
    #include "../exprLexer.h"

    int yylex();

    string filename;

    #define YYERROR_VERBOSE 1
    void yyerror(const char *msg) {
        cout << filename << ": " << ": " << msg << '\n';
    }

%}

%token decNum
%token hexNum
%token binNum
%token Char
%token String
%token Iden
%token opAdd "+"
%token opMul "*"
%token opDiv "div"
%token opSub "-"
%token opMod "mod"
%token opPow "^"
%token Error
%token Eol
%token opBra "["
%token clBra "]"
%token Comma ","
%token Colon ":"
%token opPar "("
%token clPar ")"
%token Assign "<-"
%token leThan "<"
%token grThan ">"
%token Equal "="
%token leEqu "<="
%token grEqu ">="
%token kwEntero
%token kwReal
%token kwCadena
%token kwBooleano
%token kwCaracter
%token kwArreglo
%token kwDe
%token kwFuncion
%token kwProcedimiento
%token kwVar
%token kwInicio
%token kwFin
%token kwFinal
%token kwSi
%token kwEntonces
%token kwSino
%token kwPara
%token kwMientras
%token kwHaga
%token kwLlamar
%token kwRepita
%token kwHasta
%token kwCaso
%token kwO
%token kwY
%token kwNo
%token kwLea
%token kwEscriba
%token kwRetorne
%token kwTipo
%token kwEs
%token kwRegistro
%token kwArchivo
%token kwSecuencial
%token kwAbrir
%token kwComo
%token kwLectura
%token kwEscritura
%token kwCerrar
%token kwLeer
%token kwEscribir
%token kwVerdadero
%token kwFalso
%token Eof

%%

PROGRAM: SUBTYPES-SEC VARIABLE-SEC SUBPROGRAM-DECL kwInicio OPT_EOL STATEMENTS Eol FIN ":"
    ;

SUBTYPES-SEC:
    ;

VARIABLE-SEC:
    ;

SUBPROGRAM-DECL:
    ;

STATEMENTS: STATEMENT STATEMENTS
    |
    ;

STATEMENT: kwLlamar
    | kwEscriba ARGS
    ;

ARGS: ARG ARGS
    | "," ARG ARGS
    |
    ;

ARG: String
    | EXPR
    ;

EXPR: EXPR "+" TERM
    | EXPR "-" TERM
    | TERM
    ;

TERM: TERM "*" FACTOR
    | TERM "div" FACTOR
    | TERM "mod" FACTOR
    | FACTOR
    ;

FACTOR: Iden
    | decNum | hexNum | binNum
    | "(" EXPR ")"
    ;

OPT_EOL: Eol
    |
    ;

FIN: kwFin
    |kwFinal
    ;

%%

using std::cerr;
using std::ifstream;
using std::stringstream;
using std::endl;

exprLexer *lex;

int yylex() {
    return static_cast<int>(lex->getNextToken());
}


int main(int argc, char* argv[]) {

    if (argc != 2) {
        std::cout << "file parameter is missing\n";
        return 1;
    }
    ifstream in(argv[1]);

    if (in.fail()) {
        std::cout << "cannot open file\n";
        return 1;
    }

    lex = new exprLexer(in);

    filename = argv[1];

    yyparse();

    return 0;
}