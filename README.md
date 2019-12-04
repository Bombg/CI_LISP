
				    Joshua Gottlieb
					Comp 232
					CiLisp
					Part 6

In this part I added support for conditional expressions, user defined variables, and variables with randomly assigned
values. To make this change I added functions evalCondNode and createCondAst, as well as expanding the evalfunc functions
in order to support read,rand, less, greater, and equal.

Functionality tested is to see rand, read, less, greater, and equal work correctly. I've also tested number inputs for
read in multiple different ways to make sure they are read correctly.






> ((let (int a (read)) (double b (read)) (c (read)) (d (read))) (print a b c d))
((let (int a (read)) (double b (read)) (c (read)) (d (read))) (print a b c d))
read := 0.1
0.1
"WARNING: precision loss in the typecasting of a number from double to int"
read := 2.2
2.2
read := -3.3
-3.3
read := +5.0
+5.0

Type: INT
Value: 0

Type: DOUBLE
Value: 2.200000

Type: DOUBLE
Value: -3.300000

Type: DOUBLE
Value: 5.000000

Type: INT
Value: 0
>
> ((let (int a (read)) (double b (read)) (c (read)) (d (read))) (print a b c d))
((let (int a (read)) (double b (read)) (c (read)) (d (read))) (print a b c d))
read := 1
1
read := 12
12
read := -12
-12
read := 3.0
3.0

Type: INT
Value: 1

Type: DOUBLE
Value: 12.000000

Type: INT
Value: -12

Type: DOUBLE
Value: 3.000000

Type: INT
Value: 1
>
> quit
quit

Process finished with exit code 0



> ((let (a 0)) (cond (less (rand) 0.5) (add a 1) (sub a 1)))
((let (a 0)) (cond (less (rand) 0.5) (add a 1) (sub a 1)))

Type: INT
Value: -1
> ((let (a 0)) (cond (less (rand) 0.5) (add a 1) (sub a 1)))
((let (a 0)) (cond (less (rand) 0.5) (add a 1) (sub a 1)))

Type: INT
Value: 1
> ((let (myA (read))(myB (rand)))(cond (less myA myB) (print myA) (print myB)))
((let (myA (read))(myB (rand)))(cond (less myA myB) (print myA) (print myB)))
read := +1
+1

Type: DOUBLE
Value: 0.783099

Type: DOUBLE
Value: 0.783099
>
> quit
quit

Process finished with exit code 0



> ((let (myA (read))(myB (read)))(cond (greater myA myB) (print myA) (print myB)))
((let (myA (read))(myB (read)))(cond (greater myA myB) (print myA) (print myB)))
read := 1.0
1.0
read := 2
2

Type: INT
Value: 2

Type: INT
Value: 2
>
> ((let (myA (read))(myB (read)))(cond (greater myA myB) (print myA) (print myB)))
((let (myA (read))(myB (read)))(cond (greater myA myB) (print myA) (print myB)))
read := 3.3
3.3
read := 5
5

Type: INT
Value: 5

Type: INT
Value: 5
>
> ((let (myA (read))(myB (read)))(cond (greater myA myB) (print myA) (print myB)))
((let (myA (read))(myB (read)))(cond (greater myA myB) (print myA) (print myB)))
read := +1
+1
read := 1.0
1.0

Type: DOUBLE
Value: 1.000000

Type: DOUBLE
Value: 1.000000
>
> ((let (myA (read))(myB (read)))(cond (equal myA myB) (print myA) (print myB)))
((let (myA (read))(myB (read)))(cond (equal myA myB) (print myA) (print myB)))
read := 4.0
4.0
read := 4
4

Type: INT
Value: 4

Type: INT
Value: 4
>
> ((let (myA (read))(myB (read)))(cond (equal myA myB) (print myA) (print myB)))
((let (myA (read))(myB (read)))(cond (equal myA myB) (print myA) (print myB)))
read := 4
4
read := 4.0
4.0

Type: INT
Value: 4

Type: INT
Value: 4
>
> quit
quit

Process finished with exit code 0


