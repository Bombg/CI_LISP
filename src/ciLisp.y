%{
    #include "ciLisp.h"
%}

%union {
    double dval;
    char *sval;
    struct ast_node *astNode;
    struct symbol_table_node *symbTable;
}

%token <sval> FUNC SYMBOL
%token <dval> INT DOUBLE
%token LPAREN RPAREN EOL QUIT LET

%type <astNode> s_expr f_expr number
%type <symbTable> let_elem let_list let_section

%%

program:
    s_expr EOL {
        fprintf(stderr, "yacc: program ::= s_expr EOL\n");
        if ($1) {
            printRetVal(eval($1));
            freeNode($1);
        }
    };

    /*
    under s expr for part 2 when 1 is finished

    symbol -- createsymbolnode $$ = create symbolnode($1)
    lparen let_section s _expr rparen --- $$ = linksymboltable($3, $2); // point symbol ast to symbol table and pass back

    let section $$ =$2
    let list
    let elem -- create symbol table nodes. Create symbol table node. Set id to symbol. Set value to s expr and return symbol table node

    6 helper functions to complete this.

    createSymbolNode() -- symbol
    linkSymbolNode() -- let_section s_expr
    let list
    let elem


    */

s_expr:
    number {
        fprintf(stderr, "yacc: s_expr ::= number\n");
        $$ = $1;
    }
    | SYMBOL {
    	$$ = createSymbolNode($1);
    	printf("got here");
    }
    | f_expr {
        $$ = $1;
    }
    | QUIT {
        fprintf(stderr, "yacc: s_expr ::= QUIT\n");
        exit(EXIT_SUCCESS);
    }
    | LPAREN let_section s_expr RPAREN {
	fprintf(stderr, "yacc: s_expr ::= LPAREN let_section s_expr RPAREN\n");
	$$ = linkSymbolTables($2, $3);
    }
    | error {
        fprintf(stderr, "yacc: s_expr ::= error\n");
        yyerror("unexpected token");
        $$ = NULL;
    };

number:
    INT {
        fprintf(stderr, "yacc: number ::= INT\n");
        $$ = createNumberNode($1, INT_TYPE);
    }
    | DOUBLE {
        fprintf(stderr, "yacc: number ::= DOUBLE\n");
        $$ = createNumberNode($1, DOUBLE_TYPE);
    };

f_expr:
    LPAREN FUNC s_expr RPAREN {
        fprintf(stderr, "yacc: s_expr ::= LPAREN FUNC expr RPAREN\n");
        $$ = createFunctionNode($2, $3, NULL);
    }
    | LPAREN FUNC s_expr s_expr RPAREN {
        fprintf(stderr, "yacc: s_expr ::= LPAREN FUNC expr expr RPAREN\n");
        $$ = createFunctionNode($2, $3, $4);
    };

let_section:
    LPAREN LET let_list RPAREN {
    	fprintf(stderr, "yacc: let_section ::= LPAREN let_list RPAREN\n");
	$$ = $3;
    };

let_list:
     let_elem {
     	fprintf(stderr, "yacc: let_list ::= LET let_elem\n");
	$$ = $1;
     }
     | let_list let_elem {
     	fprintf(stderr, "yacc: let_list ::= let_list let_elem\n");
	$$ = linkSymbolTables($1, $2);
     };
let_elem:
     LPAREN SYMBOL s_expr RPAREN {
     	fprintf(stderr, "yacc: let_elem ::= LPAREN SYMBOL s_expr RPAREN\n");
     	$$ = createSymbolTable($2, $3);
     }

%%

