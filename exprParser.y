%code requires {
    #include "ast.h"
    typedef ASTNode* YYSTYPE;
    #define YYSTYPE_IS_DECLARED
}

%{
    #include "../exprLexer.h"

    int yylex();
    string getText();

    string filename;

    #define YYERROR_VERBOSE 1
    void yyerror(const char *msg) {
        cout << filename << ": " << ": " << msg << '\n';
    }

%}

%token Num
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
%token NotEq "<>"
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

PROGRAM: SUBTYPES-SEC OPT_EOL VARIABLE-SEC OPT_EOL SUBPROGRAM-DECL kwInicio OPT_EOL STATEMENTS FIN OPT_EOL
    { 
        StmtList l;
        BlockStmt *b = new BlockStmt(l);
        b->addStmt((Statement*)$8);
        $$ = b;
        cout << b->toString(); }
    ;

SUBTYPES-SEC: SUBTYPE-DECL
    ;

SUBTYPE-DECL: SUBTYPE-DECL kwTipo Iden kwEs TYPE Eol
    |
    ;

TYPE: kwEntero
    | kwBooleano
    | kwCaracter
    | ARRAY-TYPE
    ;

ARRAY-TYPE: kwArreglo "[" Num "]" kwDe TYPE
    ;

VARIABLE-SEC: VARIABLE-DECL
    ;

VARIABLE-DECL: VARIABLE-DECL TYPE ID_1 Eol
    |
    ;

ID_1: Iden IDS
    ;

IDS: IDS "," Iden
    |
    ;

SUBPROGRAM-DECL: SUBPROGRAM-DECL SUBPROGRAM-HEADER Eol VARIABLE-SEC kwInicio OPT_EOL STATEMENTS kwFin Eol
    |
    ;

SUBPROGRAM-HEADER: FUNC-HEADER
    | PROC-HEADER
    ;

FUNC-HEADER: kwFuncion Iden ARGUMENT-DECLS ":" TYPE
    ;

PROC-HEADER: kwProcedimiento Iden ARGUMENT-DECLS
    ;

ARGUMENT-DECLS: "(" ARGUMENT-DECL ")"
    |
    ;

ARGUMENT-DECL: kwVar TYPE Iden MORE_ARGUMENT
    | TYPE Iden MORE_ARGUMENT
    ;

MORE_ARGUMENT:  "," kwVar Iden
    | "," TYPE Iden
    |
    ;

STATEMENTS: STATEMENTS STATEMENT Eol {
    StmtList l;
    BlockStmt *b = new BlockStmt(l);
    $$ = b;
    if ($1 != nullptr) b->addStmt((Statement*)$1);
    b->addStmt((Statement*)$2); }
    | { $$ = nullptr; }
    ;

STATEMENT: LVALUE "<-" EXPR { $$ = new assignStmt(((IdenExpr*)$1)->var_name ,(Expr*)$3); }
    | kwLlamar Iden OPT_FUNC
    | kwEscriba ARGS { $$ = new printStmt((BlockExpr*)$2); }
    | kwLea LVALUE
    | kwRetorne OPT_EXPR { $$ = new returnStmt((Expr*)$2); }
    | SI_STMT
    | kwMientras EXPR OPT_EOL kwHaga Eol STATEMENT_1 kwFin kwMientras
    | kwRepita Eol STATEMENT_1 kwHasta EXPR
    | kwPara LVALUE "<-" EXPR kwHasta EXPR kwHaga Eol STATEMENT_1 kwFin kwPara
    ;

STATEMENT_1: STATEMENT Eol STATEMENTS
    ;

SI_STMT: kwSi EXPR OPT_EOL kwEntonces OPT_EOL STATEMENT_1 OPT_SINOSI kwFin kwSi
    ;

OPT_SINOSI: kwSino OPT_SINOSI2
    |
    ;

OPT_SINOSI2: kwSi EXPR OPT_EOL kwEntonces STATEMENT_1
    | STATEMENT_1
    ;

LVALUE: Iden LVALUE_p { $$ = new IdenExpr(getText()); }
    ;

LVALUE_p: "[" EXPR "]"
    |
    ;

OPT_FUNC: "(" OPT_EXPRS ")"
    |
    ;

OPT_EXPRS: OPT_EXPRS EXPR ","
    | OPT_EXPRS EXPR
    |
    ;

ARGS: ARG MORE_ARGS {
    ExprList l;
    BlockExpr *b = new BlockExpr(l);
    b->addExpr((Expr*)$1);
    if ($2 != nullptr) b->addExpr((Expr*)$2);
    $$ = b; }
    ;

MORE_ARGS: "," ARG MORE_ARGS {
    ExprList l;
    BlockExpr *b = new BlockExpr(l);
    b->addExpr((Expr*)$2);
    $$ = b;
    if ($3 != nullptr) b->addExpr((Expr*)$3); }
    | { $$ = nullptr; }
    ;

ARG: String { $$ = new StringExpr(getText()); }
    | EXPR { $$ = $1; }
    ;

OPT_EXPR: EXPR { $$ = $1; }
    | { $$ = nullptr; }
    ;

EXPR: TERM "=" EXPR  { $$ = new EquExpr((Expr*)$1, (Expr*)$3); }
    | TERM "<>" EXPR { $$ = new NeqExpr((Expr*)$1, (Expr*)$3); }
    | TERM "<=" EXPR { $$ = new LeqExpr((Expr*)$1, (Expr*)$3); }
    | TERM ">=" EXPR { $$ = new GeqExpr((Expr*)$1, (Expr*)$3); }
    | TERM "<" EXPR  { $$ = new LsrExpr((Expr*)$1, (Expr*)$3); }
    | TERM ">" EXPR  { $$ = new GrtExpr((Expr*)$1, (Expr*)$3); }
    | TERM { $$ = $1; }
    ;

TERM: TERM "+" TERM2 { $$ = new AddExpr((Expr*)$1, (Expr*)$3); }
    | TERM "-" TERM2 { $$ = new SubExpr((Expr*)$1, (Expr*)$3); }
    | TERM kwO TERM2 { $$ = new OrExpr((Expr*)$1, (Expr*)$3); }
    | TERM2 { $$ = $1; }
    ;

TERM2: TERM2 "*" TERM3  { $$ = new MulExpr((Expr*)$1, (Expr*)$3); }
    | TERM2 "div" TERM3 { $$ = new DivExpr((Expr*)$1, (Expr*)$3); }
    | TERM2 "mod" TERM3 { $$ = new ModExpr((Expr*)$1, (Expr*)$3); }
    | TERM2 kwY TERM3   { $$ = new AndExpr((Expr*)$1, (Expr*)$3); }
    | TERM3             { $$ = $1; }
    ;

TERM3: TERM3 "^" TERM4 { $$ = new PowExpr((Expr*)$1, (Expr*)$3); }
    | TERM4            { $$ = $1; }
    ;

TERM4: kwNo FACTOR { $$ = new NotExpr((Expr*)$2); }
    | "-" FACTOR   { $$ = new NegExpr((Expr*)$2); }
    | FACTOR       { $$ = $1; }
    ;

FACTOR: Num        { $$ = new NumExpr(stoi(getText())); }
    | Char         { $$ = new CharExpr(getText()[0]);}
    | BOOL         { $$ = $1; }
    | "(" EXPR ")" { $$ = $2; }
    | RVALUE       { $$ = $1; }
    ;

RVALUE: Iden RVALUE2
    ;

RVALUE2: "[" Num "]"
    | OPT_FUNC
    ;

OPT_EOL: Eol
    |
    ;

BOOL: kwVerdadero { $$ = new BoolExpr(true); }
    | kwFalso     { $$ = new BoolExpr(false); }
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

string getText() {
    return lex->getText();
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