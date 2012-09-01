Picolisp — small Scheme subset interpreter. Designed, first of all, for calculating factorials.

Usage:
* cd picolisp
* make
* ./lambda

And you're in REPL. Now you can define your own factorial function. Just like this:
* (define fact (x) (if (eq x 0) 1 (* x (fact (dec x)))))
Done. Now you can calculate factorials:
* (fact 15)

TODO:
* Split current shitty parser to good lexer and recursive parser.
* Add lambdas to first-class objects.
* TCO.
* Add minimalistic standard library.