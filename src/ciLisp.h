#ifndef __cilisp_h_
#define __cilisp_h_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#include "ciLispParser.h"

int yyparse(void);

int yylex(void);

void yyerror(char *);

// Enum of all operators.
// must be in sync with funcs in resolveFunc()
typedef enum oper {
    NEG_OPER, // 0
    ABS_OPER,
    EXP_OPER,
    SQRT_OPER,
    ADD_OPER,
    SUB_OPER,
    MULT_OPER,
    DIV_OPER,
    REMAINDER_OPER,
    LOG_OPER,
    POW_OPER,
    MAX_OPER,
    MIN_OPER,
    EXP2_OPER,
    CBRT_OPER,
    HYPOT_OPER,
    READ_OPER,
    RAND_OPER,
    PRINT_OPER,
    EQUAL_OPER,
    LESS_OPER,
    GREATER_OPER,
    CUSTOM_OPER =255
} OPER_TYPE;

OPER_TYPE resolveFunc(char *);

// Types of Abstract Syntax Tree nodes.
// Initially, there are only numbers and functions.
// You will expand this enum as you build the project.
typedef enum {
    NUM_NODE_TYPE,
    FUNC_NODE_TYPE,
    SYMBOL_NODE_TYPE,
    COND_NODE_TYPE
} AST_NODE_TYPE;


// Types of numeric values
typedef enum {
    INT_TYPE,
    DOUBLE_TYPE,
    NO_TYPE
} NUM_TYPE;

typedef enum {
    VARIABLE_TYPE,
    LAMBDA_TYPE,
    ARG_TYPE
}SYMBOL_TYPE;

// Node to store a number.
typedef struct {
    NUM_TYPE type;
    union{
        double dval;
        long ival;
    } value;
} NUM_AST_NODE;

// Values returned by eval function will be numbers with a type.
// They have the same structure as a NUM_AST_NODE.
// The line below allows us to give this struct another name for readability.
typedef NUM_AST_NODE RET_VAL;

// Node to store a function call with its inputs
typedef struct {
    OPER_TYPE oper;
    char* ident; // only needed for custom functions
    struct ast_node *opList;
    int numOps;
} FUNC_AST_NODE;

// Generic Abstract Syntax Tree node. Stores the type of node,
// and reference to the corresponding specific node (initially a number or function call).

typedef struct symbol_ast_node {
    char *ident;
}SYMBOL_AST_NODE;

typedef struct stack_node {
    struct ast_node *val;
    struct stack_node *next;
}STACK_NODE;

typedef struct symbol_table_node {
    SYMBOL_TYPE type;
    NUM_TYPE val_type;
    char *ident;
    struct ast_node *val;
    STACK_NODE *stack;
    struct symbol_table_node *next;
}SYMBOL_TABLE_NODE;

typedef struct {
    struct ast_node *cond;
    struct ast_node *ifTrue;
    struct ast_node *ifFalse;

}COND_AST_NODE;

//typedef struct arg_table_node {
//    char *ident;
//    NUM_TYPE type;
//    struct ast_node *val;
//    struct arg_table_node *next;
//}ARG_TABLE_NODE;

typedef struct ast_node {
    AST_NODE_TYPE type;
    SYMBOL_TABLE_NODE *symbolTable;
    struct ast_node *parent;
    union {
        NUM_AST_NODE number;
        FUNC_AST_NODE function;
        SYMBOL_AST_NODE symbol;
        COND_AST_NODE condition;
    } data;
    struct ast_node *next;
} AST_NODE;






AST_NODE *createNumberNode(char *typeNum, double value, NUM_TYPE type);
AST_NODE *createFunctionNode(char *funcName, AST_NODE *opList);
void freeNode(AST_NODE *node);
RET_VAL eval(AST_NODE *node);
RET_VAL evalNumNode(NUM_AST_NODE *numNode);
RET_VAL evalFuncNode(FUNC_AST_NODE *funcNode);
NUM_TYPE binaryOpHelper(RET_VAL *op1, RET_VAL *op2);
SYMBOL_TABLE_NODE *createSymbolTable(char *typeNum, char *symbol, AST_NODE *value, SYMBOL_TYPE type);
SYMBOL_TABLE_NODE *createSymbArgList(char *symbol, SYMBOL_TABLE_NODE *oldHead, SYMBOL_TYPE type);
SYMBOL_TABLE_NODE *createSymbLambda(char *typeNum, char *symbol, SYMBOL_TABLE_NODE *argList, AST_NODE *sExpr, SYMBOL_TYPE type);
SYMBOL_TABLE_NODE *linkSymbolTables(SYMBOL_TABLE_NODE *list, SYMBOL_TABLE_NODE *elem);
AST_NODE *linkAstSymbTable(SYMBOL_TABLE_NODE *tableHead, AST_NODE *funcNode);
AST_NODE *createSymbAstNode(char *symbol);
RET_VAL findSymbolValue(AST_NODE *node, char *symbol);
AST_NODE *linkOpNodes(AST_NODE *newHead, AST_NODE *oldHead );
RET_VAL *evalUnary(FUNC_AST_NODE *funcNode);
RET_VAL *evalBinary(FUNC_AST_NODE *funcNode);
RET_VAL *evalNary(FUNC_AST_NODE *funcNode);
AST_NODE *createCondAst(AST_NODE *condition, AST_NODE *ifTrue, AST_NODE *ifFalse);
RET_VAL evalCondNode(COND_AST_NODE *node);
RET_VAL *evalCustomFunc(AST_NODE *opNodes, AST_NODE *funcNode, char *lambda );
void fillArgs(AST_NODE *values, SYMBOL_TABLE_NODE *args);


void printRetVal(RET_VAL val);

#endif
