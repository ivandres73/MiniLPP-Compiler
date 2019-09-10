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

%token Num "num"
%token Char "char"
%token String "string"
%token Iden "iden"
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
%token kwEntero "entero"
%token kwReal
%token kwCadena
%token kwBooleano "boolean"
%token kwCaracter "caracter"
%token kwArreglo "arreglo"
%token kwDe "de"
%token kwFuncion "funcion"
%token kwProcedimiento "procedimiento"
%token kwVar "var"
%token kwInicio "inicio"
%token kwFin "fin"
%token kwFinal "final"
%token kwSi "si"
%token kwEntonces "entonces"
%token kwSino "sino"
%token kwPara "para"
%token kwMientras "mientras"
%token kwHaga "haga"
%token kwLlamar "llamar"
%token kwRepita "repita"
%token kwHasta "hasta"
%token kwCaso
%token kwO "o"
%token kwY "y"
%token kwNo "no"
%token kwLea "lea"
%token kwEscriba "escriba"
%token kwRetorne "retorne"
%token kwTipo "tipo"
%token kwEs "es"
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
%token kwVerdadero "verdadero"
%token kwFalso "falso"
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

SUBTYPE-DECL: SUBTYPE-DECL "tipo" "iden" "es" TYPE Eol
    |
    ;

TYPE: "entero"
    | "boolean"
    | "caracter"
    | ARRAY-TYPE
    ;

ARRAY-TYPE: "arreglo" "[" "num" "]" "de" TYPE
    ;

VARIABLE-SEC: VARIABLE-DECL
    ;

VARIABLE-DECL: VARIABLE-DECL TYPE ID_1 Eol
    |
    ;

ID_1: "iden" IDS
    ;

IDS: IDS "," "iden"
    |
    ;

SUBPROGRAM-DECL: SUBPROGRAM-DECL SUBPROGRAM-HEADER Eol VARIABLE-SEC kwInicio OPT_EOL STATEMENTS "fin" Eol
    |
    ;

SUBPROGRAM-HEADER: FUNC-HEADER
    | PROC-HEADER
    ;

FUNC-HEADER: "funcion" "iden" ARGUMENT-DECLS ":" TYPE
    ;

PROC-HEADER: "procedimiento" "iden" ARGUMENT-DECLS
    ;

ARGUMENT-DECLS: "(" ARGUMENT-DECL ")"
    |
    ;

ARGUMENT-DECL: "var" TYPE "iden" MORE_ARGUMENT
    | TYPE "iden" MORE_ARGUMENT
    ;

MORE_ARGUMENT:  "," "var" "iden"
    | "," TYPE "iden"
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
    | "llamar" "iden" OPT_FUNC { $$ = new callStmt("default id"); }
    | "escriba" ARGS { $$ = new printStmt((BlockExpr*)$2); }
    | "lea" LVALUE { $$ = new readStmt(((IdenExpr*)$2)->var_name); }
    | "retorne" OPT_EXPR { $$ = new returnStmt((Expr*)$2); }
    | SI_STMT
    | "mientras" EXPR OPT_EOL "haga" Eol STATEMENT_1 "fin" "mientras"
    | "repita" Eol STATEMENT_1 "hasta" EXPR
    | "para" LVALUE "<-" EXPR "hasta" EXPR "haga" Eol STATEMENT_1 "fin" "para"
    ;

STATEMENT_1: STATEMENT Eol STATEMENTS
    ;

SI_STMT: kwSi EXPR OPT_EOL "entonces" OPT_EOL STATEMENT_1 OPT_SINOSI "fin" kwSi
    ;

OPT_SINOSI: "sino" OPT_SINOSI2
    |
    ;

OPT_SINOSI2: kwSi EXPR OPT_EOL "entonces" STATEMENT_1
    | STATEMENT_1
    ;

LVALUE: "iden" LVALUE_p { $$ = new IdenExpr(getText()); }
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

ARG: "string" { $$ = new StringExpr(getText()); }
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
    | TERM "o" TERM2 { $$ = new OrExpr((Expr*)$1, (Expr*)$3); }
    | TERM2 { $$ = $1; }
    ;

TERM2: TERM2 "*" TERM3  { $$ = new MulExpr((Expr*)$1, (Expr*)$3); }
    | TERM2 "div" TERM3 { $$ = new DivExpr((Expr*)$1, (Expr*)$3); }
    | TERM2 "mod" TERM3 { $$ = new ModExpr((Expr*)$1, (Expr*)$3); }
    | TERM2 "y" TERM3   { $$ = new AndExpr((Expr*)$1, (Expr*)$3); }
    | TERM3             { $$ = $1; }
    ;

TERM3: TERM3 "^" TERM4 { $$ = new PowExpr((Expr*)$1, (Expr*)$3); }
    | TERM4            { $$ = $1; }
    ;

TERM4: "no" FACTOR { $$ = new NotExpr((Expr*)$2); }
    | "-" FACTOR   { $$ = new NegExpr((Expr*)$2); }
    | FACTOR       { $$ = $1; }
    ;

FACTOR: "num"        { $$ = new NumExpr(stoi(getText())); }
    | "char"         { $$ = new CharExpr(getText()[0]); }
    | BOOL         { $$ = $1; }
    | "(" EXPR ")" { $$ = $2; }
    | RVALUE       { $$ = $1; }
    ;

RVALUE: "iden" RVALUE2
    ;

RVALUE2: "[" "num" "]"
    | OPT_FUNC
    ;

OPT_EOL: Eol
    |
    ;

BOOL: "verdadero" { $$ = new BoolExpr(true); }
    | "falso"     { $$ = new BoolExpr(false); }
    ;

FIN: "fin"
    |"final"
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