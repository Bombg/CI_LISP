#include "ciLisp.h"

void yyerror(char *s){
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
        "print",
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
AST_NODE *createNumberNode(char *typeNum, double value, NUM_TYPE type)
{
    AST_NODE *node;
    size_t nodeSize;

    // allocate space for the fixed sie and the variable part (union)
    nodeSize = sizeof(AST_NODE);
    if ((node = calloc(nodeSize, 1)) == NULL)
        yyerror("Memory allocation failed!");

    // TODO set the AST_NODE's type, assign values to contained NUM_AST_NODE - Done

    node->type = NUM_NODE_TYPE;

    if(typeNum == NULL || (strcmp("int",typeNum) == 0 && type == INT_TYPE)
        || (strcmp("double",typeNum) == 0 && type == DOUBLE_TYPE))
    {

        node->data.number.type = type;

        if(type == INT_TYPE)
        {
            node->data.number.value.ival = value;
        }
        else
        {
            node->data.number.value.dval = value;
        }
    }
    else if(strcmp("int",typeNum) == 0)
    {
        node->data.number.type = INT_TYPE;
        printf("\"WARNING: precision loss in the typecasting of a number from double to int\"\n");

        if(fmod(value,1) >= 0.5)
        {
            double newval = value + .5;
            node->data.number.value.ival = newval;

        }
        else
        {
            node->data.number.value.ival = value;
        }
    }
    else
    {
        node->data.number.type = DOUBLE_TYPE;
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
AST_NODE *createFunctionNode(char *funcName, AST_NODE *opList)
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
   if(strcmp(funcName,"read") != 0 && strcmp(funcName,"rand") != 0)
   {
       node->data.function.opList = opList;
       AST_NODE *nextNode;
       AST_NODE *previous;
       nextNode = opList;
       previous = node;

       while(nextNode != NULL)
       {
           nextNode->parent = previous;
           node->data.function.numOps++;
           previous = nextNode;
           nextNode = nextNode->next;
       }
   }
    node->data.function.oper = resolveFunc(funcName);

   if(node->data.function.oper != CUSTOM_OPER)
   {
       free(funcName);
   }
   else
   {
       node->data.function.ident = funcName;
   }



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

        freeNode(node->data.function.opList);

        // Free up identifier string if necessary
        if (node->data.function.oper == CUSTOM_OPER)
        {
            free(node->data.function.ident);
        }

    }
    if(node->symbolTable != NULL)
    {
        SYMBOL_TABLE_NODE *nextNode = node->symbolTable->next;
        SYMBOL_TABLE_NODE *curNode = node->symbolTable;

        if(curNode->val)
        {
            freeNode(curNode->val);
        }

        while(curNode)
        {
            free(curNode);

            if(nextNode)
            {
                curNode = nextNode;
                nextNode = nextNode->next;
            }
            else
            {
                curNode = NULL;
            }

        }
    }
    if(node->type == COND_NODE_TYPE)
    {
        freeNode(node->data.condition.cond);
        freeNode(node->data.condition.ifFalse);
        freeNode(node->data.condition.ifTrue);

    }
    if(node->type == SYMBOL_NODE_TYPE)
    {
        free(node->data.symbol.ident);
        //free(&node->data.symbol);
    }
    if(node->next)
    {
        freeNode(node->next);
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
            break;
        case SYMBOL_NODE_TYPE:
            result = findSymbolValue(node, node->data.symbol.ident);
            break;
        case COND_NODE_TYPE:
            result = evalCondNode(&node->data.condition);
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

    RET_VAL *result;

    // TODO populate result with the result of running the function on its operands. - Done
    // SEE: AST_NODE, AST_NODE_TYPE, FUNC_AST_NODE

    switch(funcNode->oper)
    {
        case NEG_OPER:
        case ABS_OPER:
        case EXP_OPER:
        case SQRT_OPER:
        case LOG_OPER:
        case EXP2_OPER:
        case CBRT_OPER:
        case READ_OPER:
        case RAND_OPER:
            result = evalUnary(funcNode);
            break;
        case ADD_OPER:
        case MULT_OPER:
        case PRINT_OPER:
            result = evalNary(funcNode);
            break;
        case SUB_OPER:
        case DIV_OPER:
        case REMAINDER_OPER:
        case POW_OPER:
        case MAX_OPER:
        case MIN_OPER:
        case HYPOT_OPER:
        case EQUAL_OPER:
        case LESS_OPER:
        case GREATER_OPER:
            result = evalBinary(funcNode);
            break;
        case CUSTOM_OPER:
            result = evalCustomFunc(funcNode->opList,funcNode->opList, funcNode->ident);

    }
    return (*result);
}

NUM_TYPE binaryOpHelper(RET_VAL *op1, RET_VAL *op2)
{
    NUM_TYPE type;
    if(op1->type != op2->type)
    {
        type = DOUBLE_TYPE;

        if(op1->type == INT_TYPE)
        {
            op1->type = DOUBLE_TYPE;
            op1->value.dval = (double)op1->value.ival;
        }
        else if (op2->type == INT_TYPE)
        {
            op2->type = DOUBLE_TYPE;
            op2->value.dval = (double)op2->value.ival;
        }
    }
    else if( op1->type == INT_TYPE && op2->type == INT_TYPE)
    {
        type = INT_TYPE;
    }
    else
    {
        type = DOUBLE_TYPE;
    }

    return type;
}

// prints the type and value of a RET_VAL

void printRetVal(RET_VAL val){
    // TODO print the type and value of the value passed in. - Done

    switch (val.type)
    {
        case INT_TYPE:
            printf("\nType: INT");
            printf("\nValue: %ld",val.value.ival);
            break;
        case DOUBLE_TYPE:
            printf("\nType: DOUBLE");
            printf("\nValue: %lf", val.value.dval);
            break;
    }

}

SYMBOL_TABLE_NODE *createSymbolTable(char* typeNum, char *symbol, AST_NODE *value, SYMBOL_TYPE type)
{
    SYMBOL_TABLE_NODE *symbNode;
    size_t nodeSize;

    nodeSize = sizeof(AST_NODE);
    if ((symbNode = calloc(nodeSize, 1)) == NULL)
    {
        yyerror("Memory allocation failed!");
    }


    symbNode->ident = symbol;
    symbNode->type = type;

    if(value->type == NUM_NODE_TYPE )
    {
        if(typeNum == NULL || (strcmp("int",typeNum) == 0 && value->data.number.type == INT_TYPE)
                              || (strcmp("double",typeNum) == 0 && value->data.number.type == DOUBLE_TYPE))
        {
            symbNode->val = value;
        }
        else
        {
            if(strcmp("int",typeNum) == 0)
            {
                printf("WARNING: precision loss in the assignment for variable %s",symbol);
                value->data.number.type = INT_TYPE;
                if(fmod(value->data.number.value.dval,1) >= 0.5)
                {

                    value->data.number.value.dval = value->data.number.value.dval + .5;
                    value->data.number.value.ival = value->data.number.value.dval;

                }
                else
                {
                    value->data.number.value.ival = value->data.number.value.dval;
                }
                symbNode->val = value;
            }
            else
            {
                value->data.number.type = DOUBLE_TYPE;
                value->data.number.value.dval = value->data.number.value.ival;
                symbNode->val = value;
            }
        }
    }
    else if( value->type == FUNC_NODE_TYPE)
    {
        RET_VAL newVal = eval(value);

        switch (newVal.type)
        {
            case INT_TYPE:
                symbNode->val = createNumberNode(typeNum, newVal.value.ival, newVal.type);
                break;
            case DOUBLE_TYPE:
                symbNode->val = createNumberNode(typeNum, newVal.value.dval, newVal.type);
                break;
        }
    }





    return symbNode;


}

SYMBOL_TABLE_NODE *createSymbArgList(char *symbol, SYMBOL_TABLE_NODE *oldHead, SYMBOL_TYPE type)
{
    SYMBOL_TABLE_NODE *argNode;
    size_t nodeSize;

    nodeSize = sizeof(AST_NODE);
    if ((argNode = calloc(nodeSize, 1)) == NULL)
    {
        yyerror("Memory allocation failed!");
    }


    argNode->ident = symbol;
    argNode->type = type;
    argNode->next = oldHead;
    argNode->val = NULL;

    return argNode;
}

SYMBOL_TABLE_NODE *createSymbLambda(char *typeNum, char *symbol, SYMBOL_TABLE_NODE *argList, AST_NODE *sExpr, SYMBOL_TYPE type)
{
    SYMBOL_TABLE_NODE *symbNode;
    size_t nodeSize;

    nodeSize = sizeof(AST_NODE);
    if ((symbNode = calloc(nodeSize, 1)) == NULL)
    {
        yyerror("Memory allocation failed!");
    }

    symbNode->type = type;
    symbNode->ident = symbol;
    symbNode->val = sExpr;
    sExpr->symbolTable = argList;


    if(typeNum != NULL)
    {
        if(strcmp(typeNum, "int") == 0)
        {
            symbNode->val_type = INT_TYPE;
        }
        else if (strcmp(typeNum, "double") == 0)
        {
            symbNode->val_type = DOUBLE_TYPE;
        }
        else
        {
            symbNode->val_type = NO_TYPE;
        }
    }



    return symbNode;


}

SYMBOL_TABLE_NODE *linkSymbolTables(SYMBOL_TABLE_NODE *list, SYMBOL_TABLE_NODE *elem)
{

    elem->next = list;

    return elem;
}



AST_NODE *linkAstSymbTable(SYMBOL_TABLE_NODE *tableHead, AST_NODE *funcNode)
{
    funcNode->symbolTable = tableHead;

    return funcNode;
}

AST_NODE *createSymbAstNode(char *symbol)
{
    AST_NODE *node;
    size_t nodeSize;
    SYMBOL_AST_NODE *symbNode;

    // allocate space (or error)
    nodeSize = sizeof(AST_NODE);
    if ((node = calloc(nodeSize, 1)) == NULL)
        yyerror("Memory allocation failed!");

    nodeSize = sizeof(SYMBOL_AST_NODE);
    if ((symbNode = calloc(nodeSize, 1)) == NULL)
        yyerror("Memory allocation failed!");

    node->type = SYMBOL_NODE_TYPE;
    symbNode->ident = symbol;


    node->data.symbol = *symbNode;

    return node;


}

RET_VAL findSymbolValue(AST_NODE *node, char *symbol) {

    if (node->symbolTable != NULL) {
        SYMBOL_TABLE_NODE *symbNode = node->symbolTable;
        RET_VAL result;

        while (symbNode != NULL) {
            if (strcmp(symbol, symbNode->ident) == 0) {
               if(symbNode->type != ARG_TYPE)
               {
                   result = symbNode->val->data.number;

                   if (symbNode->val->data.number.type == INT_TYPE) {
                       result.type = INT_TYPE;
                       result.value.ival = symbNode->val->data.number.value.ival;
                   } else {
                       result.type = DOUBLE_TYPE;
                       result.value.dval = symbNode->val->data.number.value.dval;
                   }
               }
               else
               {
                   result = symbNode->stack->val->data.number;

                   if(symbNode->stack->val->data.number.type == INT_TYPE)
                   {
                       result.type = INT_TYPE;
                   }
                   else
                   {
                       result.type = DOUBLE_TYPE;
                   }
               }


                return result;
            }
            symbNode = symbNode->next;
        }
    }
    if (node->parent != NULL)
    {
        return findSymbolValue(node->parent, symbol);
    }
    else
    {
        printf("ERROR, Value for %s not found!", symbol);
        exit(0);
    }


}


AST_NODE *linkOpNodes(AST_NODE *newHead, AST_NODE *oldHead )
{
    newHead->next = oldHead;
    oldHead->parent = newHead;

    return newHead;
}

RET_VAL *evalUnary(FUNC_AST_NODE *funcNode)
{
    size_t  retvalSize;
    retvalSize = sizeof(RET_VAL);
    RET_VAL *result;
    if ((result = calloc(retvalSize, 1)) == NULL)
        yyerror("Memory allocation failed!");
    char tempScan[65];




    if(funcNode->numOps > 1)
    {
        printf("Too many operators for this function!");
        exit(0);
    }
    else
    {
        RET_VAL op1 = eval(funcNode->opList);
        int numType = op1.type;
        switch (funcNode->oper)
        {
            case NEG_OPER:
                switch (numType)
                {
                    case INT_TYPE:
                        result->type = INT_TYPE;
                        result->value.ival = -1 * op1.value.ival;
                        break;
                    case DOUBLE_TYPE:
                        result->type = DOUBLE_TYPE;
                        result->value.dval = -1 * op1.value.dval;
                        break;
                }
                break;
            case ABS_OPER:
                switch (numType)
                {
                    case INT_TYPE:
                        result->type = INT_TYPE;
                        result->value.ival =  labs(op1.value.ival);
                        break;
                    case DOUBLE_TYPE:
                        result->type = DOUBLE_TYPE;
                        result->value.dval =  fabs(op1.value.dval);
                        break;
                }
                break;
            case EXP_OPER:
                switch (numType)
                {
                    case INT_TYPE:
                        result->type = INT_TYPE;
                        result->value.ival = exp(op1.value.ival);
                        break;
                    case DOUBLE_TYPE:
                        result->type = DOUBLE_TYPE;
                        result->value.dval = exp(op1.value.dval);
                        break;
                }
                break;
            case SQRT_OPER:
                switch (numType)
                {
                    case INT_TYPE:
                        result->type = INT_TYPE;
                        result->value.ival = sqrt(op1.value.ival);
                        break;
                    case DOUBLE_TYPE:
                        result->type = DOUBLE_TYPE;
                        result->value.dval = sqrt(op1.value.dval);
                        break;
                }
                break;
            case LOG_OPER:
                switch (numType)
                {
                    case INT_TYPE:
                        result->type = INT_TYPE;
                        result->value.ival = log(op1.value.ival);
                        break;
                    case DOUBLE_TYPE:
                        result->type = DOUBLE_TYPE;
                        result->value.dval = log(op1.value.dval);
                        break;
                }
                break;
            case EXP2_OPER:
                switch (numType)
                {
                    case INT_TYPE:
                        result->type = INT_TYPE;
                        result->value.ival = powl(2,op1.value.ival);
                        break;
                    case DOUBLE_TYPE:
                        result->type = DOUBLE_TYPE;
                        result->value.dval = pow(2,op1.value.dval);
                        break;
                }
                break;
            case CBRT_OPER:
                switch (numType)
                {
                    case INT_TYPE:
                        result->type = INT_TYPE;
                        result->value.ival = cbrtl(op1.value.ival);
                        break;
                    case DOUBLE_TYPE:
                        result->type = DOUBLE_TYPE;
                        result->value.dval = cbrt(op1.value.dval);
                        break;
                }
                break;
            case RAND_OPER:
                result->type = DOUBLE_TYPE;
                result->value.dval = ((double)rand() / (double)RAND_MAX);
                break;
            case READ_OPER:
                printf("read := ");
                scanf("%s",tempScan);
                bool isdouble = false;
                int ASCIIZERO = 48;
                int ASCIININE = 57;
                bool isNeg = false;
                bool isSign = false;
                int startInd = 0;

                for(int i = 0 ; i < 65 && tempScan[i] != '\0'; i++)
                {
                    if(tempScan[i] == '.')
                    {
                        if(isdouble || i == 0)
                        {
                            printf("Bad input for number\n");
                            exit(0);
                        }
                        isdouble = true;
                    }
                    if((tempScan[i] == '-' || tempScan[i] == '+') && i == 0)
                    {
                        startInd = 1;
                        if(tempScan[i] == '-')
                        {
                            isNeg =  true;
                        }
                    }
                    else if((tempScan[i] < ASCIIZERO || tempScan[i] > ASCIININE) && tempScan[i] != '.' )
                    {
                        printf("Bad input for number\n");
                        exit(0);
                    }
                }



                if(isdouble)
                {
                    result->type = DOUBLE_TYPE;
                    bool pointFound;
                    double decPlace = 10;

                    result->value.dval = (tempScan[startInd] - ASCIIZERO);
                    for(int i = startInd + 1; i < 65 && tempScan[i] != '\0'; i++)
                    {
                        if(tempScan[i] == '.')
                        {
                            pointFound = true;
                        }
                        else
                        {
                            if(!pointFound)
                            {
                                result->value.dval *=10;
                                result->value.dval+= (tempScan[i] - ASCIIZERO);
                            }
                            else
                            {
                                result->value.dval += (tempScan[i] - ASCIIZERO) / decPlace;
                                decPlace *= 10;
                            }
                        }


                    }
                    if(isNeg)
                    {
                        result->value.dval *= -1;
                    }
                }
                else
                {
                    result->type = INT_TYPE;
                    result->value.ival  = (tempScan[startInd] - ASCIIZERO);
                    for(int i = startInd + 1; i < 65 && tempScan[i] != '\0'; i++)
                    {
                        result->value.ival *= 10;
                        result->value.ival += (tempScan[i] - ASCIIZERO);
                    }
                    if(isNeg)
                    {
                        result->value.ival *= -1;
                    }
                }



                break;
        }
    }

    return result;

}
RET_VAL *evalBinary(FUNC_AST_NODE *funcNode)
{
    size_t  retvalSize;
    retvalSize = sizeof(RET_VAL);
    RET_VAL *result;
    if ((result = calloc(retvalSize, 1)) == NULL)
        yyerror("Memory allocation failed!");
    RET_VAL op1 = eval(funcNode->opList);
    RET_VAL op2 = eval(funcNode->opList->next);

    if(funcNode->numOps != 2)
    {
        printf("Too many or too few operators for this function type");
        exit(0);
    }
    else
    {
        switch (funcNode->oper)
        {
            case SUB_OPER:
                switch (binaryOpHelper(&op1, &op2))
                {
                    case INT_TYPE:
                        result->value.ival = op1.value.ival - op2.value.ival;
                        break;
                    case DOUBLE_TYPE:
                        result->type = DOUBLE_TYPE;
                        result->value.dval = op1.value.dval - op2.value.dval;
                        break;
                }
                break;
            case DIV_OPER:
                switch (binaryOpHelper(&op1, &op2))
                {
                    case INT_TYPE:
                        result->value.ival = op1.value.ival / op2.value.ival;
                        break;
                    case DOUBLE_TYPE:
                        result->type = DOUBLE_TYPE;
                        result->value.dval = op1.value.dval / op2.value.dval;
                        break;
                }
                break;
            case REMAINDER_OPER:
                switch (binaryOpHelper(&op1, &op2))
                {
                    case INT_TYPE:
                        result->value.ival = op1.value.ival % op2.value.ival;
                        break;
                    case DOUBLE_TYPE:
                        result->type = DOUBLE_TYPE;
                        result->value.dval =fmod(op1.value.dval, op2.value.dval);
                }
                break;
            case POW_OPER:
                switch (binaryOpHelper(&op1, &op2))
                {
                    case INT_TYPE:
                        result->value.ival = pow(op1.value.ival, op2.value.ival);
                        break;
                    case DOUBLE_TYPE:
                        result->type = DOUBLE_TYPE;
                        result->value.dval = pow(op1.value.dval, op2.value.dval);
                        break;
                }
                break;
            case MAX_OPER:
                switch (binaryOpHelper(&op1, &op2))
                {
                    case INT_TYPE:
                        result->value.ival = (op1.value.ival > op2.value.ival ?
                                             op1.value.ival : op2.value.ival );
                        break;
                    case DOUBLE_TYPE:
                        result->type = DOUBLE_TYPE;
                        result->value.dval = (op1.value.dval > op2.value.dval ?
                                             op1.value.dval : op2.value.dval );
                        break;
                }
                break;
            case MIN_OPER:
                switch (binaryOpHelper(&op1, &op2))
                {
                    case INT_TYPE:
                        result->value.ival = (op1.value.ival < op2.value.ival ?
                                             op1.value.ival : op2.value.ival );
                        break;
                    case DOUBLE_TYPE:
                        result->type = DOUBLE_TYPE;
                        result->value.dval = (op1.value.dval < op2.value.dval ?
                                             op1.value.dval : op2.value.dval );
                        break;
                }
                break;
            case HYPOT_OPER:
                switch (binaryOpHelper(&op1, &op2))
                {
                    case INT_TYPE:
                        result->type = DOUBLE_TYPE;
                        op1.value.dval = op1.value.ival;
                        op2.value.dval = op2.value.ival;
                        result->value.dval = hypotl(op1.value.dval, op2.value.dval);
                        break;
                    case DOUBLE_TYPE:
                        result->type = DOUBLE_TYPE;
                        result->value.dval = hypotl(op1.value.dval, op2.value.dval);
                        break;
                }
                break;
            case GREATER_OPER:
                if(op1.type == op2.type)
                {
                    if(op1.type == INT_TYPE)
                    {
                        result->value.ival = op1.value.ival > op2.value.ival ? 1 : 0 ;
                    }
                    else
                    {
                        result->value.ival =op1.value.dval > op2.value.dval ? 1 : 0 ;
                    }
                }
                else
                {
                    if(op1.type == INT_TYPE)
                    {
                        result->value.ival = op1.value.ival > op2.value.dval ? 1 : 0 ;
                    }
                    else
                    {
                        result->value.dval =op1.value.dval > op2.value.ival ? 1 : 0 ;
                    }
                }
                break;
            case LESS_OPER:
                if(op1.type == op2.type)
                {
                    if(op1.type == INT_TYPE)
                    {
                        result->value.ival = op1.value.ival < op2.value.ival ? 1 : 0 ;
                    }
                    else
                    {
                        result->value.ival =op1.value.dval < op2.value.dval ? 1 : 0 ;
                    }
                }
                else
                {
                    if(op1.type == INT_TYPE)
                    {
                        result->value.ival = op1.value.ival < op2.value.dval ? 1 : 0 ;
                    }
                    else
                    {
                        result->value.dval =op1.value.dval < op2.value.ival ? 1 : 0 ;
                    }
                }
                break;
            case EQUAL_OPER:
                if(op1.type == op2.type)
                {
                    if(op1.type == INT_TYPE)
                    {
                        result->value.ival = op1.value.ival == op2.value.ival ? 1 : 0 ;
                    }
                    else
                    {
                        result->value.ival =op1.value.dval == op2.value.dval ? 1 : 0 ;
                    }
                }
                else
                {
                    if(op1.type == INT_TYPE)
                    {
                        result->value.ival = op1.value.ival == op2.value.dval ? 1 : 0 ;
                    }
                    else
                    {
                        result->value.dval =op1.value.dval == op2.value.ival ? 1 : 0 ;
                    }
                }
                break;
        }
    }

return result;

}
RET_VAL *evalNary(FUNC_AST_NODE *funcNode)
{
    size_t  retvalSize;
    retvalSize = sizeof(RET_VAL);
    RET_VAL *result;
    if ((result = calloc(retvalSize, 1)) == NULL)
        yyerror("Memory allocation failed!");

    AST_NODE *tempNode = funcNode->opList;
    int isDoub = 0;
    RET_VAL op = eval(tempNode);

    if(op.type == DOUBLE_TYPE)
    {
        result->type = DOUBLE_TYPE;
        isDoub = 1;
    }
    else
    {
        result->type = INT_TYPE;
    }

    RET_VAL tempRet;



    switch(funcNode->oper)
    {
        case ADD_OPER:

            for(int i = 0; i < funcNode->numOps; i++)
            {
                switch(isDoub)
                {
                    case 0:
                        if(op.type == DOUBLE_TYPE)
                        {
                            result->value.dval = result->value.ival;
                            result->value.dval += op.value.dval;
                            result->type = DOUBLE_TYPE;
                            isDoub = 1;
                        }
                        else
                        {
                            result->value.ival += op.value.ival;
                        }
                        break;
                    case 1:
                        if(op.type == DOUBLE_TYPE)
                        {
                            result->value.dval += op.value.dval;
                        }
                        else
                        {
                            result->value.dval += op.value.ival;
                        }
                        break;
                }
                tempNode = tempNode->next;
                op = eval(tempNode);
            }
            break;
        case MULT_OPER:
            if(isDoub == 0)
            {
                result->value.ival = op.value.ival;
            }
            else
            {
                result->value.dval = op.value.dval;
            }
            tempNode = tempNode->next;
            op = eval(tempNode);
            for(int i = 1; i < funcNode->numOps; i++)
            {
                switch(isDoub)
                {
                    case 0:
                        if(op.type == DOUBLE_TYPE)
                        {
                            result->value.dval = result->value.ival;
                            result->value.dval *= op.value.dval;
                            result->type = DOUBLE_TYPE;
                            isDoub = 1;
                        }
                        else
                        {
                            result->value.ival *= op.value.ival;
                        }
                        break;
                    case 1:
                        if(op.type == DOUBLE_TYPE)
                        {
                            result->value.dval *= op.value.dval;
                        }
                        else
                        {
                            result->value.dval *= op.value.ival;
                        }
                        break;
                }
                tempNode = tempNode->next;
                op = eval(tempNode);
            }
            break;
        case PRINT_OPER:

            for(int i = 0; i < funcNode->numOps; i++)
            {
                tempRet = eval(tempNode);
                printRetVal(tempRet);
                printf("\n");
                tempNode = tempNode->next;

            }

            if(isDoub == 0)
            {

                result->value.ival = op.value.ival;
            }
            else
            {
                result->value.dval = op.value.dval;
            }
            break;
    }


    return result;
}
RET_VAL *evalCustomFunc(AST_NODE *opNodes, AST_NODE *node, char *lambda)
{
    if (node->symbolTable != NULL) {
        SYMBOL_TABLE_NODE *symbNode = node->symbolTable;

        size_t  retvalSize;
        retvalSize = sizeof(RET_VAL);
        RET_VAL *result;
        if ((result = calloc(retvalSize, 1)) == NULL)
            yyerror("Memory allocation failed!");

        while (symbNode != NULL) {
            if (strcmp(lambda, symbNode->ident) == 0) {


                fillArgs(opNodes,symbNode->val->symbolTable);
                (*result) = eval(symbNode->val);

                symbNode->val->symbolTable->stack = symbNode->val->symbolTable->stack->next;
                symbNode->val->symbolTable->next->stack = symbNode->val->symbolTable->next->stack->next;

                return result;
            }
            symbNode = symbNode->next;
        }
    }
    if (node->parent != NULL)
    {
        return evalCustomFunc(opNodes, node->parent, lambda);
    }
    else
    {
        printf("ERROR, Value for %s not found!", lambda);
        exit(0);
    }
}

void fillArgs(AST_NODE *values, SYMBOL_TABLE_NODE *args)
{

   STACK_NODE *xStackNode;
   STACK_NODE *yStackNode;
   size_t nodeSize;

    nodeSize = sizeof(STACK_NODE);
    if ((xStackNode = calloc(nodeSize, 1)) == NULL ||(yStackNode = calloc(nodeSize, 1)) == NULL)
    {
        yyerror("Memory allocation failed!");
    }
    values->data.number = eval(values);
    values->next->data.number = eval(values->next);

    args->stack = xStackNode;
    args->next->stack = yStackNode;

    args->stack->val = values;
    args->next->stack->val = values->next;


}

AST_NODE *createCondAst(AST_NODE *condition, AST_NODE *ifTrue, AST_NODE *ifFalse)
{
    size_t  condSize;
    condSize = sizeof(AST_NODE);
    AST_NODE *node;
    if ((node = calloc(condSize, 1)) == NULL)
        yyerror("Memory allocation failed!");


    node->type = COND_NODE_TYPE;
    node->data.condition.cond = condition;
    node->data.condition.cond->parent = node;
    node->data.condition.ifTrue = ifTrue;
    node->data.condition.ifTrue->parent = node;
    node->data.condition.ifFalse = ifFalse;
    node->data.condition.ifFalse->parent = node;

    return node;

}

RET_VAL evalCondNode(COND_AST_NODE *node)
{
    RET_VAL result;

    result = evalFuncNode(&node->cond->data.function);

    if(result.value.ival == 1)
    {
        result = eval(node->ifTrue);
    }
    else
    {
        result = eval(node->ifFalse);
    }

    return result;
}
