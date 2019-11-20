
				    Joshua Gottlieb
					Comp 232
					CiLisp
					Part 4

This part of the CiLisp program implements a print function. To allow for this I added "print" to the function section
of the lex file, edited the ciLisp.c funcnames array with "print", and filled out the print section of the switch in
the evalfuncnode function.

There are no known issues at this point. The changes made weren't extensive so any (unknown) bugs are probably from
previous parts.

In the output below I've tested printing different parts of an evaluation, printed evaluations themselves, and printed
evaluations that included symbols.







> (add 2 (print (add 2 1)))

Type: INT
Value: 3

Type: INT
Value: 5
> (mult 3 (print (mult 5 10)))

Type: INT
Value: 50

Type: INT
Value: 150
> ((let (a 2))(print (add a 2)))

Type: INT
Value: 4

Type: INT
Value: 4
> quit

Process finished with exit code 0


> (add ((let (a ((let (b 2)) (mult b (sqrt 10))))) (div a 2)) (print ((let (c 5)) (sqrt c))))

Type: DOUBLE
Value: 2.236068

Type: DOUBLE
Value: 5.398346
> (add (print((let (a ((let (b 2)) (mult b (sqrt 10))))) (div a 2))) (print ((let (c 5)) (sqrt c))))

Type: DOUBLE
Value: 3.162278

Type: DOUBLE
Value: 2.236068

Type: DOUBLE
Value: 5.398346
> quit

Process finished with exit code 0

