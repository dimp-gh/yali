# A few thoughts about lexical scopes implementation.


Data structure for environment.
Requirements:
- Mapping from string to SExpression (or any other type (void*, kekeke)).
  * lookup :: string -> SExpression
  * insert :: string x SExpression -> ok
  * remove :: string -> ok
- Fast fork.
- Fast deleting.


Block.
Block is a sequence of SExpressions.
Evaluating the block: we evaluate all expressions from left to right and returning the evaluation result of last SExpression.
Block can take arguments.
In other words, block is a multiline lambda with its own scope.
Functions defined in block are only visible in that block.
 

Maybe lambda should be powered up to block.
Ok, i'm assuming that for future.


Typical block-using evaluation algorithm:

function evaluate(expression, environment)
	switch (expression type)
	case int, string, float, nil:
		return expression
	case id:
		return lookup(id, environment)

	case funcall:
		[we've got function call]
		[funcall is non-empty list, where head is either id or block]
		[below assuming, that head of list is *id*, and *args* is the rest of list]
		case core function application:
			get function from core environment by id,
			apply function to arguments and environment,
			get result,
			return result
		case non-core function application:
			get function from current environment by id,
			eval arguments (if function isn't core function),
			fork current environment,
			add mappings from function arguments to evaluated arguments to forked environment,
			[if some mapping in environment is already reserved — delete it before adding new mapping]
			eval function body (with modified forked environment),
			get result,
			delete forked environment,
			return result

	case block:
		evaluate all expressions, that block contains,
		return result of evaluating last expression


all core functions have the following signature:
  <core function> :: SExpression x Environment -> SExpression x Environment

all foreign (non-core) functions
