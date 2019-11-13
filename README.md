
				    Joshua Gottlieb
					Comp 232
					CLisp
					Part 1




In this part of the project I've filled out all of the TODO sections.  This includes createNumberNode, createFunctionNode, eval, evalNumNode, evalFuncNode, and printRetVal.

I've also created a helper function called binaryOpHelper that returns number types to help with the evalFuncNode switch statements and to help make them a little more compact.

I've tested all of the functions, as well as nesting functions, and so far as far as I can tell there is no issues.

Here is a sample run.



> > (add (exp (sub 3.5 (sqrt 15))) (log (remainder (pow 3 (sqrt 20)) 5.2)))
>
> Type: DOUBLE Value: 0.545349
> > (add 1 1)
>
> Type: INT Value: 2
> > (add (add 1 1) (add 2 1))
>
> Type: INT Value: 5
> > (add 2.2 3)
>
> Type: DOUBLE Value: 5.200000
> > (sub 1 100)
>
> Type: INT Value: -99
> > (sub .1 100) ERROR: invalid character: >>.<<
>
> Type: INT Value: -99
> > (sub 0.1 100)
>
> Type: DOUBLE Value: -99.900000
> > (sqrt 15)
>
> Type: DOUBLE Value: 3.872983
> > (mult 5 2.2)
>
> Type: DOUBLE Value: 11.000000
> > (div 6 3)
>
> Type: INT Value: 2
> > (div 4 5)
>
> Type: INT Value: 0
> > (div 4.0 5.0)
>
> Type: DOUBLE Value: 0.800000
> > (rem 5 12) ERROR: invalid character: >>r<< ERROR: invalid character: >>e<< ERROR: invalid character: >>m<<
>
> > (remainder 5 12)
>
> Type: INT Value: 5
> > (remainder 12 5)
>
> Type: INT Value: 2
> > quit
>
> Process finished with exit code 0
>
>
> > (hypot 3 4)
>
> Type: DOUBLE Value: 5.000000
> > (cbrt 8)
>
> Type: INT Value: 2
> > (exp2 2)
>
> Type: INT Value: 4
> > (min 3 4)
>
> Type: INT Value: 3
> > (max 3 4)
>
> Type: INT Value: 4
> > quit
>
> Process finished with exit code 0

