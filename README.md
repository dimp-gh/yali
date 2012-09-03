Yali (Yep, Another Lisp Interpreter) is _extremely_ small Lisp interpreter. Designed, first of all, for calculating factorials.

Usage:  
`make`  
`./lambda`  

And you're in REPL. Now you can define your own factorial function. Just like this:  
`>>> (define fact (x) (if (eq x 0) 1 (* x (fact (dec x)))))`  
Done. Now you can calculate factorials:  
`>>> (fact 15)`
`Eval = 1307674368000`

TODO:
* Split current shitty parser to good lexer and recursive parser.
* Add lambdas as first-class objects.
* TCO.
* Add string data type.
* Add minimalistic standard library.
* Create own data types, instead of using C types.
* Liquidate memory leaks in evaluator's core.
* Add scopes, preferably lexical.
