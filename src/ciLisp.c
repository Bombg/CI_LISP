#include "ciLisp.h"

void yyerror(char *s) {
    fprintf(stderr, "\nERROR: %s\n", s);
    // note stderr that normally defaults to stdout, but can be redirected: ./src 2> src.log
    // CLion will display stderr in a different color from stdin and stdout
}

// Array of string values for operations.
// Must be in sync with funcs in the OPER_TYPE enum in order for resolveFunc to work.
char *funcNames[] = {
        "neg",
        "abs",
        "exp",
        "sqrt",
        "add",
        "sub",
        "mult",
        "div",
        "remainder",
        "log",
        "pow",
        "max",
        "min",
        "exp2",
        "cbrt",
        "hypot",
        "read",
        "rand",
        "print",
        "equal",
        "less",
        "greater",
        ""
};

OPER_TYPE resolveFunc(char *funcName)
{
    int i = 0;
    while (funcNames[i][0] != '\0')
    {
        if (strcmp(funcNames[i], funcName) == 0)
            return i;
        i++;
    }
    return CUSTOM_OPER;
}

// Called when an INT or DOUBLE token is encountered (see ciLisp.l and ciLisp.y).
// Creates an AST_NODE for the number.
// Sets the AST_NODE's type to number.
// Populates the value of the contained NUMBER_AST_NODE with the argument value.
// SEE: AST_NODE, NUM_AST_NODE, AST_NODE_TYPE.
AST_NODE *createNumberNode(double value, NUM_TYPE type)
{
    AST_NODE *node;
    size_t nodeSize;

    // allocate space for the fixed sie and the variable part (union)
    nodeSize = sizeof(AST_NODE);
    if ((node = calloc(nodeSize, 1)) == NULL)
        yyerror("Memory allocation failed!");

    // TODO set the AST_NODE's type, assign values to contained NUM_AST_NODE - Done

    node->type = NUM_NODE_TYPE;
    node->data.number.type = type;

    if(type == INT_TYPE)
    {
        node->data.number.value.ival = value;
    }
    else
    {
        node->data.number.value.dval = value;
    }



    return node;
}

// Called when an f_expr is created (see ciLisp.y).
// Creates an AST_NODE for a function call.
// Sets the created AST_NODE's type to function.
// Populates the contained FUNC_AST_NODE with:
//      - An OPER_TYPE (the enum identifying the specific function being called)
//      - 2 AST_NODEs, the operands
// SEE: AST_NODE, FUNC_AST_NODE, AST_NODE_TYPE.
AST_NODE *createFunctionNode(char *funcName, AST_NODE *op1, AST_NODE *op2)
{
    AST_NODE *node;
    size_t nodeSize;

    // allocate space (or error)
    nodeSize = sizeof(AST_NODE);
    if ((node = calloc(nodeSize, 1)) == NULL)
        yyerror("Memory allocation failed!");

    // TODO set the AST_NODE's type, populate contained FUNC_AST_NODE - Done
    // NOTE: you do not need to populate the "ident" field unless the function is type CUSTOM_OPER.
    // When you do have a CUSTOM_OPER, you do NOT need to allocate and strcpy here.
    // The funcName will be a string identifier for which space should be allocated in the tokenizer.
    // For CUSTOM_OPER functions, you should simply assign the "ident" pointer to the passed in funcName.
    // For functions other than CUSTOM_OPER, you should free the funcName after you're assigned the OPER_TYPE.

    node->type = FUNC_NODE_TYPE;
    node->data.function.op1 = op1;
    node->data.function.op2 = op2;
    node->data.function.oper = resolveFunc(funcName);

    free(funcName);

    return node;
}

// Called after execution is done on the base of the tree.
// (see the program production in ciLisp.y)
// Recursively frees the whole abstract syntax tree.
// You'll need to update and expand freeNode as the project develops.
void freeNode(AST_NODE *node)
{
    if (!node)
        return;

    if (node->type == FUNC_NODE_TYPE)
    {
        // Recursive calls to free child nodes
        freeNode(node->data.function.op1);
        freeNode(node->data.function.op2);

        // Free up identifier string if necessary
        if (node->data.function.oper == CUSTOM_OPER)
        {
            free(node->data.function.ident);
        }
    }

    free(node);
}

// Evaluates an AST_NODE.
// returns a RET_VAL storing the the resulting value and type.
// You'll need to update and expand eval (and the more specific eval functions below)
// as the project develops.
RET_VAL eval(AST_NODE *node)
{
    if (!node)
        return (RET_VAL){INT_TYPE, NAN};

    RET_VAL result = {INT_TYPE, NAN}; // see NUM_AST_NODE, because RET_VAL is just an alternative name for it.

    // TODO complete the switch. - Done
    // Make calls to other eval functions based on node type.
    // Use the results of those calls to populate result.
    switch (node->type)
    {
        case FUNC_NODE_TYPE:
            result = evalFuncNode(&node->data.function); // return by value and not by reference
            break;
        case NUM_NODE_TYPE:
            result = evalNumNode(&node->data.number);
        default:
            yyerror("Invalid AST_NODE_TYPE, probably invalid writes somewhere!");
    }

    return result;
}  

// returns a pointer to the NUM_AST_NODE (aka RET_VAL) referenced by node.
// DOES NOT allocate space for a new RET_VAL.
RET_VAL evalNumNode(NUM_AST_NODE *numNode) // If different types of data. Upgrade to more complex types.
{
    if (!numNode)
        return (RET_VAL){INT_TYPE, NAN};

    RET_VAL result = {INT_TYPE, NAN};

    // TODO populate result with the values stored in the node. - Done
    // SEE: AST_NODE, AST_NODE_TYPE, NUM_AST_NODE

    if(numNode->type == INT_TYPE)
    {
        result.value.ival = numNode->value.ival;
    }
    else
    {
        result.type = DOUBLE_TYPE;
        result.value.dval = numNode->value.dval;
    }


    return result;
}


RET_VAL evalFuncNode(FUNC_AST_NODE *funcNode)
{
    if (!funcNode)
        return (RET_VAL){INT_TYPE, NAN};

    RET_VAL result = {INT_TYPE, NAN};

    // TODO populate result with the result of running the function on its operands.
    // SEE: AST_NODE, AST_NODE_TYPE, FUNC_AST_NODE

    switch(funcNode->oper)
    {
        case NEG_OPER:
            switch (funcNode->op1->type)
            {
                case INT_TYPE:
                    result.type = INT_TYPE;
                    result.value.ival = -1 * funcNode->op1->data.number.value.ival;
                    break;
                case DOUBLE_TYPE:
                    result.type = DOUBLE_TYPE;
                    result.value.dval = -1 * funcNode->op1->data.number.value.dval;
                    break;
            }
            break;
        case ABS_OPER:
            switch (funcNode->op1->type)
            {
                case INT_TYPE:
                    result.type = INT_TYPE;
                    result.value.ival = labs(funcNode->op1->data.number.value.ival);
                    break;
                case DOUBLE_TYPE:
                    result.type = DOUBLE_TYPE;
                    result.value.dval = fabs(funcNode->op1->data.number.value.dval);
            }
            break;
        case EXP_OPER:
            switch (funcNode->op1->type)
            {
                case INT_TYPE:
                    result.type = INT_TYPE;
                    result.value.ival = exp(funcNode->op1->data.number.value.ival);
                    break;
                case DOUBLE_TYPE:
                    result.type = DOUBLE_TYPE;
                    result.value.dval = exp(funcNode->op1->data.number.value.dval);
                    break;
            }
            break;
        case SQRT_OPER:
            switch (funcNode->op1->type)
            {
                case INT_TYPE:
                    result.type = INT_TYPE;
                    result.value.ival = sqrt(funcNode->op1->data.number.value.ival);
                    break;
                case DOUBLE_TYPE:
                    result.type = DOUBLE_TYPE;
                    result.value.dval = sqrt(funcNode->op1->data.number.value.dval);
                    break;
            }
            break;
        case ADD_OPER:
            switch (binaryOpHelper(funcNode, result))
            {
                case INT_TYPE:
                    result.value.ival = funcNode->op1->data.number.value.ival + funcNode->op2->data.number.value.ival;
                    break;
                case DOUBLE_TYPE:
                    result.value.dval = funcNode->op1->data.number.value.dval + funcNode->op2->data.number.value.dval;
                    break;
            }
            break;
        case SUB_OPER:
            switch (binaryOpHelper(funcNode, result))
            {
                case INT_TYPE:
                    result.value.ival = funcNode->op1->data.number.value.ival - funcNode->op2->data.number.value.ival;
                    break;
                case DOUBLE_TYPE:
                    result.value.dval = funcNode->op1->data.number.value.dval - funcNode->op2->data.number.value.dval;
                    break;
            }
            break;
        case MULT_OPER:
            switch (binaryOpHelper(funcNode, result))
            {
                case INT_TYPE:
                    result.value.ival = funcNode->op1->data.number.value.ival * funcNode->op2->data.number.value.ival;
                    break;
                case DOUBLE_TYPE:
                    result.value.dval = funcNode->op1->data.number.value.dval * funcNode->op2->data.number.value.dval;
                    break;
            }
            break;
        case DIV_OPER:
            switch (binaryOpHelper(funcNode, result))
            {
                case INT_TYPE:
                    result.value.ival = funcNode->op1->data.number.value.ival / funcNode->op2->data.number.value.ival;
                    break;
                case DOUBLE_TYPE:
                    result.value.dval = funcNode->op1->data.number.value.dval / funcNode->op2->data.number.value.dval;
                    break;
            }
            break;
        case REMAINDER_OPER:
            switch (binaryOpHelper(funcNode, result))
            {
                case INT_TYPE:
                    result.value.ival = funcNode->op1->data.number.value.ival % funcNode->op2->data.number.value.ival;
                    break;
                case DOUBLE_TYPE:
                    result.value.dval =fmod(funcNode->op1->data.number.value.dval, funcNode->op2->data.number.value.dval);
                    break;
            }
            break;
        case LOG_OPER:
            switch(funcNode->op1->type)
            {
                case INT_TYPE:
                    result.value.ival = (int)log(funcNode->op1->data.number.value.ival);
                    break;
                case DOUBLE_TYPE:
                    result.value.dval = log(funcNode->op1->data.number.value.dval);
                    break;
            }
            break;
        case POW_OPER:
            switch (binaryOpHelper(funcNode, result))
            {
                case INT_TYPE:
                    result.value.ival = pow(funcNode->op1->data.number.value.ival, funcNode->op2->data.number.value.ival);
                    break;
                case DOUBLE_TYPE:
                    result.value.dval = pow(funcNode->op1->data.number.value.dval, funcNode->op2->data.number.value.dval);
                    break;
            }
            break;
        case MAX_OPER:
            switch (binaryOpHelper(funcNode, result))
            {
                case INT_TYPE:
                    result.value.ival = (funcNode->op1->data.number.value.ival > funcNode->op2->data.number.value.ival ?
                                         funcNode->op1->data.number.value.ival : funcNode->op2->data.number.value.ival );
                    break;
                case DOUBLE_TYPE:
                    result.value.dval = (funcNode->op1->data.number.value.dval > funcNode->op2->data.number.value.dval ?
                                         funcNode->op1->data.number.value.dval : funcNode->op2->data.number.value.dval );
                    break;
            }
            break;
        case MIN_OPER:
            switch (binaryOpHelper(funcNode, result))
            {
                case INT_TYPE:
                    result.value.ival = (funcNode->op1->data.number.value.ival < funcNode->op2->data.number.value.ival ?
                                         funcNode->op1->data.number.value.ival : funcNode->op2->data.number.value.ival );
                    break;
                case DOUBLE_TYPE:
                    result.value.dval = (funcNode->op1->data.number.value.dval < funcNode->op2->data.number.value.dval ?
                                         funcNode->op1->data.number.value.dval : funcNode->op2->data.number.value.dval );
                    break;
            }
            break;
        case EXP2_OPER:

            break;
        case CBRT_OPER: // cube root

            break;
        case HYPOT_OPER:

            break;
        case READ_OPER:

            break;

        case RAND_OPER:

            break;
        case PRINT_OPER:

            break;
        case EQUAL_OPER:

            break;
        case LESS_OPER:

            break;
        case GREATER_OPER:

            break;
    }





    return result;
}

int binaryOpHelper(FUNC_AST_NODE *funcNode, RET_VAL result)
{
    int type;
    if(funcNode->op1->type != funcNode->op2->type)
    {
        result.type = DOUBLE_TYPE;
        type = DOUBLE_TYPE;
    }
    else if (funcNode->op1->type == INT_TYPE)
    {
        result.type = INT_TYPE;
        type = INT_TYPE;
    }
    else
    {
        result.type = DOUBLE_TYPE;
        type =DOUBLE_TYPE;
    }

    return type;
}

// prints the type and value of a RET_VAL

void printRetVal(RET_VAL val)
{
    // TODO print the type and value of the value passed in.
}
