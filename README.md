
				    Joshua Gottlieb
					Comp 232
					CLisp
					Part 3





The changes made to this part of CiLisp is the addition of explicit data types in the
forms of int and doubles. The grammar was expanded to have numbers with optional types 
before them. Types in this context are strings 'int' and 'double' . let_elem was also 
expanded to have an optional type before it. 

The lex file was edited to allow for a type token, which has a string value. The Yak file
was expanded to allow for the grammar additions mentioned above. createSymbolTable, and 
createNumberNode were expanded to take in a type parameter. Both of these functions do what
they did previously if there is no type declared, but if there is rounds up or down
if needed.

As far as I know there aren't any issues. There are a few functions that I still output as 
a double since I feel like they're not as useful if you get a rounded int return.

Below is an output where I tested changing data types opposite of their
literal, same as their literal. There's also declaring a symbol as a type, 
and declaring a number as a type.

> ((let (int a 1.25))(add a 1))
WARNING: precision loss in the assignment for variable a
Type: INT
Value: 2
> ((let (int a 1.5))(add a 1))
WARNING: precision loss in the assignment for variable a
Type: INT
Value: 3
> (add ((let (int a ((let (int b 2)) (mult b (sqrt 10))))) (div a 2)) ((let (c 5)) (sqrt c)))
"WARNING: precision loss in the typecasting of a number from double to int"
Type: DOUBLE
Value: 5.236068
> ((let (double first (sub 5 1)) (second 2)) (add (pow 2 first) (sqrt second)))

Type: DOUBLE
Value: 17.414214
> ((let (int first (sub 5 1)) (second 2)) (add (pow 2 first) (sqrt second)))

Type: DOUBLE
Value: 17.414214
> 
> ((let (double a 1.25))(add a 1))

Type: DOUBLE
Value: 2.250000

> (add int 2 double 2)

Type: DOUBLE
Value: 4.000000
> (div int 5 int 2)

Type: INT
Value: 2
> quit

Process finished with exit code 0

