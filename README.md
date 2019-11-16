
				    Joshua Gottlieb
					Comp 232
					CLisp
					Part 2




This part of the project expanded the grammar to allow for symbols and assigning values
to those symbols. To allow for this I've expanded the Lex file to look for symbol,let,
and letter tokens. The YAK file was also expanded to hold the new productions. These new
productions include let_section, let_list, and let_elem. To help build the AST tree I've 
created new helper functions which include createSymbolTable, linkSymbolTables,
linkAstSymbTable, createSymbAstNode, and lastly findSymbolValue.

I've tested this part of the program thoroughly, and as far as I know there isn't any issues.
I've tested with the sample inputs that were provided, and with some of my own inputs. All of the
math functions were tested in the previous part, so the main things that need testing is making sure that
symbols are getting values, and that those symbol values can be accessed when they are used in a math
function. Another issue is making sure that if a symbol is given a function as a value, then to make sure
that function is evaluated and that the symbol table gets the proper value.

Test runs are below



>> (add ((let (abcd 1)) (sub 3 abcd)) 4)

>Type: INT
>Value: 6
>> (mult ((let (a 1) (b 2)) (add a b)) (sqrt 2))

>Type: DOUBLE
>Value: 4.242641
>> (add ((let (a ((let (b 2)) (mult b (sqrt 10))))) (div a 2)) ((let (c 5)) (sqrt c)))

>Type: DOUBLE
>Value: 5.398346
>> ((let (first (sub 5 1)) (second 2)) (add (pow 2 first) (sqrt second)))

>Type: DOUBLE
>Value: 17.414214
>> ((let (a ((let (c 3) (d 4)) (mult c d)))) (sqrt a))

>Type: DOUBLE
>Value: 3.464102
>> 

> >((let (x 2)(y 10)) (mult x y))

>Type: INT
>Value: 20
>> ((let (z 50)(y 50))(div z y))

>Type: INT
>Value: 1
>> quit


