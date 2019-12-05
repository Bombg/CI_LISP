
				    Joshua Gottlieb
					Comp 232
					CiLisp
					Part 7




 else
   {
       if (node->args != NULL) {
           ARG_TABLE_NODE *argNode = node->args;

           while (argNode != NULL) {
               if (strcmp(symbol, argNode->ident) == 0) {
                   RET_VAL result = argNode->val->data.number;

                   if (argNode->val->data.number.type == INT_TYPE) {
                       result.type = INT_TYPE;
                       result.value.ival = argNode->val->data.number.value.ival;
                   } else {
                       result.type = DOUBLE_TYPE;
                       result.value.dval = argNode->val->data.number.value.dval;
                   }
                   return result;
               }
               argNode = argNode->next;
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