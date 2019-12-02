
				    Joshua Gottlieb
					Comp 232
					CiLisp
					Part 5

The changes made to this part of CiLisp is added support for functions that can take a large number of operators. 
The functions that can take large numbers of operators are print, add, and mult. I've also added checks to each function
that throws errors if a function is given too few or too many operators for its type. To support this change I split 
evalFunc() into three different functions, these functions are, evalUnary, EvalBinary, and EvalUnary.


In the first sample I've tested the basic operation of the functions that take more than two operators. Along with this 
I've made sure that if the data type changes as the program continues then the output at the end reflects the data type.
Since I've rewritten my evalfunc I'm also testing previous operators to make sure they're working the way they're supposed
to work. I've also checked to see if nested functions still work along with declaring data types.





> (add 1 2 3 4 5 6 7)

Type: INT
Value: 28
> (add 1 2 3.0 4 5 6 7)

Type: DOUBLE
Value: 28.000000
> (mult 2.0 2 2 2)

Type: DOUBLE
Value: 16.000000
> (print 1 2 3 4 5 6)

Type: INT
Value: 1

Type: INT
Value: 2

Type: INT
Value: 3

Type: INT
Value: 4

Type: INT
Value: 5

Type: INT
Value: 6

Type: INT
Value: 1
> quit

Process finished with exit code 0

/Users/joshua.gottlieb824/CLionProjects/CI_LISP/cmake-build-debug/cilisp

> ((let (a 2))(neg a))

Type: INT
Value: -2
> ((let (a 2))(abs a))

Type: INT
Value: 2
> ((let (a 2))(exp a))

Type: INT
Value: 7
> ((let (a 2))(sqrt a))

Type: INT
Value: 1
> quit

Process finished with exit code 0

> ((let (double a 2)(b 3))(mult a b))

Type: DOUBLE
Value: 6.000000
> ((let (double a 2)(b 3))(add a b))

Type: DOUBLE
Value: 5.000000
> (add ((let (a ((let (b 2)) (mult b (sqrt 10))))) (div a 2)) ((let (c 5)) (sqrt c)))

Type: INT
Value: 5
> (add ((let (double a ((let (b 2)) (mult b (sqrt 10))))) (div a 2)) ((let (c 5)) (sqrt c)))

Type: DOUBLE
Value: 5.000000
> ((let (c 5)) (sqrt c))

Type: INT
Value: 2
> ((let (double a ((let (b 2)) (mult b (sqrt 10))))) (div a 2))

Type: DOUBLE
Value: 3.000000
> (add ((let (double a ((let (b 2.0)) (mult b (sqrt 10.0))))) (div a 2)) ((let (c 5.0)) (sqrt c)))

Type: DOUBLE
Value: 5.398346
> quit


> (add ((let (a ((let (b 2)) (mult b (sqrt 10))))) (div a 2)) (print ((let (c 5)) (sqrt c))))

Type: INT
Value: 2

Type: INT
Value: 5
> (add (print((let (a ((let (b 2)) (mult b (sqrt 10))))) (div a 2))) (print ((let (c 5)) (sqrt c))))

Type: INT
Value: 3

Type: INT
Value: 2

Type: INT
Value: 5
> quit

