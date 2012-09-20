Yali (Yep, Another Lisp Interpreter) is _extremely_ small Lisp interpreter. Designed, first of all, for calculating factorials.

Usage:  
`make`  
`./repl`  

And you're in REPL. Now you can define your own factorial function. Just like this:  
`>>> (define (fact x) (if (= x 0) 1 (* x (fact (- x 1)))))`  
Done. Now you can calculate factorials:  
`>>> (fact 15)`  
`Eval = 1307674368000`  
Another example:  
`>>> (define (fibo x) (if (= x 1) 1 (if (= x 2) 1 (+ (fibo (- x 1)) (fibo (- x 2))))))`

TODO:
* Split current shitty parser to good lexer and recursive parser. ✔
* Add lambdas as first-class objects. ✔
* TCO.
* Add _string_ data type. ✔
* Add _floating-point number_ data type. ✔
* Add minimalistic standard library of built-in functions. ✔
 - List operations (car, cdr, cons, map, reduce). ✔
 - Type conversions and predicates. ✔
 - Basic arithmetic, logic and comparison operations. ✔
 - I/O functions.
 - Time measurement functions.
 - Execution flow control function.
* Create own data types, instead of using C types.
* Liquidate memory leaks in evaluator's core. Preferrably, by building GC.
* Add scopes, preferably lexical.
* Error reports.
* Static typing. (I'm not kidding.) (Actually, i don't think it's possible. But i definitely can make a new form of `define`, `typed-define`, which will define a function with incapsulated type checking. And every time this function will be called, it will typecheck its arguments by itself. Example of syntax: `(define-typed (factorial x) (:: (int? x)) (...))`).
