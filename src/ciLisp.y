%{
    #include "ciLisp.h"
%}

%union {
    double dval;
    char *sval;
    struct ast_node *astNode;
    struct symbol_table_node *symbTable;
}

%token <sval> FUNC SYMBOL TYPE
%token <dval> INT_LITERAL DOUBLE_LITERAL
%token LPAREN RPAREN EOL QUIT LET COND

%type <astNode> s_expr f_expr number s_expr_list
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


s_expr:
    number {
        fprintf(stderr, "yacc: s_expr ::= number\n");
        $$ = $1;
    }
    | SYMBOL {
    	fprintf(stderr, "yacc: s_expr ::= SYMBOL\n");
    	$$ = createSymbAstNode($1);

    }
    | f_expr {
    	fprintf(stderr, "yacc: s_expr ::= f_expr\n");
        $$ = $1;
    }
    | QUIT {
        fprintf(stderr, "yacc: s_expr ::= QUIT\n");
        exit(EXIT_SUCCESS);
    }
    | LPAREN let_section s_expr RPAREN {
	fprintf(stderr, "yacc: s_expr ::= LPAREN let_section s_expr RPAREN\n");
	$$ = linkAstSymbTable($2, $3);
    }
    | LPAREN COND s_expr s_expr s_expr RPAREN {
	$$ = createCondAst($3, $4, $5);
    }
    | error {
        fprintf(stderr, "yacc: s_expr ::= error\n");
        yyerror("unexpected token");
        $$ = NULL;
    };

number:
    INT_LITERAL {
        fprintf(stderr, "yacc: number ::= INT_LITERAL\n");
        $$ = createNumberNode(NULL, $1, INT_TYPE);
    }
    | DOUBLE_LITERAL {
        fprintf(stderr, "yacc: number ::= DOUBLE_LITERAL\n");
        $$ = createNumberNode(NULL, $1, DOUBLE_TYPE);
    }
    | TYPE INT_LITERAL {
    	fprintf(stderr, "yacc: number ::= TYPE INT_LITERAL\n");
        $$ = createNumberNode($1, $2, INT_TYPE);
    }
    | TYPE DOUBLE_LITERAL {
    	fprintf(stderr, "yacc: number ::= TYPE DOUBLE_LITERAL\n");
        $$ = createNumberNode($1, $2, DOUBLE_TYPE);
    };

f_expr:
     LPAREN FUNC s_expr_list RPAREN {
     fprintf(stderr, "yacc: f_expr ::= LPAREN FUNC s_expr_list RPAREN\n");
	$$ = createFunctionNode($2, $3);
     };

s_expr_list:
     s_expr s_expr_list {
     	fprintf(stderr, "yacc: s_expr_list ::= s_expr s_expr_list\n");
	$$ = linkOpNodes($1, $2);
     }
     | s_expr {
     	fprintf(stderr, "yacc: s_expr_list ::= s_expr\n");
	$$ = $1;
     }
     | /* empty */
     ;

let_section:
	LPAREN let_list RPAREN {
    	fprintf(stderr, "yacc: let_section ::= LPAREN let_list RPAREN\n");
	$$ = $2;
    }
    |
    ; /* empty */

let_list:
     LET let_elem {
     	fprintf(stderr, "yacc: let_list ::= LET let_elem\n");
	$$ = $2;
     }
     | let_list let_elem {
     	fprintf(stderr, "yacc: let_list ::= let_list let_elem\n");
	$$ = linkSymbolTables($1, $2);
     };
let_elem:
     LPAREN SYMBOL s_expr RPAREN {
     	fprintf(stderr, "yacc: let_elem ::= LPAREN SYMBOL s_expr RPAREN\n");
     	$$ = createSymbolTable(NULL, $2, $3);
     }
     | LPAREN TYPE SYMBOL s_expr RPAREN {
     	fprintf(stderr, "yacc: let_elem ::= LPAREN SYMBOL s_expr RPAREN\n");
        $$ = createSymbolTable($2, $3, $4);
     };

arg_list: SYMBOL arg_list {

     }

%%

