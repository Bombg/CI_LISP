
				   
					Comp 232
					CiLisp
					Part 7 & 8
					
In this part of CiLisp I added support for custom functions as well as letting a function
call itself. TO support this I've added several functions including
evalCustomFunc, createSymbArgList, createSymbLambda, and fillargs. I've also
added extra functions to free memory after each operation. While most of the
memory has been freed, there is a few spots I haven't figured out out to
free and there's on in particular that seg faults when I attempt to free it (commented out).

Below are sample runs that test the ability to parse and evaluate custom functions
as as functions calling themselves. Along with the tests is functions of differing data types.

Since this is the last part of the project I've also included sample runs that include
sample code from previous parts so you can see that they're still working as well.
					
					
					

> ((let (int a 1)(f lambda (x y) (add x y)))(f 2 (f a 3)))
((let (int a 1)(f lambda (x y) (add x y)))(f 2 (f a 3)))

Type: INT
Value: 6
> ((let (double myFunc lambda (x y) (mult (add x 5) (sub y 2)))) (sub (myFunc 3 5) (myFunc 2 3)))
((let (double myFunc lambda (x y) (mult (add x 5) (sub y 2)))) (sub (myFunc 3 5) (myFunc 2 3)))

Type: DOUBLE
Value: 17.000000
> ((let (f lambda (x y) (add x y)))(f (sub 5 2) (mult 2 3))) 
((let (f lambda (x y) (add x y)))(f (sub 5 2) (mult 2 3))) 

Type: INT
Value: 9
> ((let (double myFunc lambda (x y) (mult (add x 5) (sub y 2)))) (sub (myFunc 3 5) 2))
((let (double myFunc lambda (x y) (mult (add x 5) (sub y 2)))) (sub (myFunc 3 5) 2))

Type: DOUBLE
Value: 22.000000
> quit
quit

Process finished with exit code 0

> ((let (int a 1)(double f lambda (x y) (mult x y)))(f 2 (f a 3)))
((let (int a 1)(double f lambda (x y) (mult x y)))(f 2 (f a 3)))

Type: DOUBLE
Value: 6.000000
> 
((let (int a 3)(int f lambda (x y) (mult x y)))(f 2 (f a 3)))
((let (int a 3)(int f lambda (x y) (mult x y)))(f 2 (f a 3)))
> 


Type: INT
Value: 18
> quit
quit
					




 /mnt/c/Users/Bombg/CLionProjects/CI_LISP/cmake-build-debug/cilisp
 
 > (add 1 2 3 4 5)
 (add 1 2 3 4 5)
 
 Type: INT
 Value: 15
 > (add ((let (abc 1)) (sub 3 abc)) 4)
 (add ((let (abc 1)) (sub 3 abc)) 4)
 
 Type: INT
 Value: 6
 > (add ((let (a ((let (b 2)) (mult b (sqrt 10))))) (div a 2)) ((let (c 5)) (sqrt c)))
 (add ((let (a ((let (b 2)) (mult b (sqrt 10))))) (div a 2)) ((let (c 5)) (sqrt c)))
 
 Type: INT
 Value: 5
 > ((let (first (sub 5 1)) (second 2)) (add (pow 2 first) (sqrt second)))
 ((let (first (sub 5 1)) (second 2)) (add (pow 2 first) (sqrt second)))
 
 Type: INT
 Value: 17
 > (add ((let (double a ((let (double b 2)) (mult b (sqrt 10))))) (div a 2)) ((let (double c 5)) (sqrt c)))
 (add ((let (double a ((let (double b 2)) (mult b (sqrt 10))))) (div a 2)) ((let (double c 5)) (sqrt c)))
 
 Type: DOUBLE
 Value: 5.236068
 > ((let (int a 1.25))(add a 1))
 ((let (int a 1.25))(add a 1))
 WARNING: precision loss in the assignment for variable a
 Type: INT
 Value: 2
 > (print 1)
 (print 1)
 
 Type: INT
 Value: 1
 
 Type: INT
 Value: 1
 > (add 1 2 3 4)
 (add 1 2 3 4)
 
 Type: INT
 Value: 10
 > ((let (int a (read)) (double b (read)) (c (read)) (d (read))) (print a b c d))
 ((let (int a (read)) (double b (read)) (c (read)) (d (read))) (print a b c d))
 read := 1
 1
 read := 2
 2
 read := 3
 3
 read := 4
 4
 
 Type: INT
 Value: 1
 
 Type: DOUBLE
 Value: 2.000000
 
 Type: INT
 Value: 3
 
 Type: INT
 Value: 4
 
 Type: INT
 Value: 1
 > 
 > quit
 quit
 
 Process finished with exit code 0
