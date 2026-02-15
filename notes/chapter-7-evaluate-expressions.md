# 7 Evaluating Expressions
Executing the syntax tree itself.

Tokens -> syntax tree -> values

**Problem**: To Execute code/expression, need to evaluate expression and
provide a value (so far).<br> 
To parse literal, operator etc. we need a corresponding chunk of code 
that knows how to evaluate that specific tree.

2 Things to note.
1. What kind of values to produce?
2. How do we organise those chunks of code?

## 7.1 Representing Values
In lox, values are:<br> 
Created by literals -> computed by expressions -> stored in variables.

Need a type in java to represent Lox objects. Lox objects can store any type
of variable. We need a way to determine which type of value our Lox object is
storing at runtime to be able to perform different operations on them.

WE GOT **java.lang.Object**!!!

## 7.2 Evaluating Expressions

**Interpreter design pattern**: Each syntax tree node has an `interpret()`
method to evaluate expressions. Not really scalable.

INSTEAD WE WILL USE THE GOOD OL' VISITOR evaluate.

### 7.2.1 Evaluating Literals
Literal is a bit of syntax that produces a value. It is not the value itself.
Literal comes from the source code. Something parsed by our parser. A value
being produced in the code is not a literal. Value is from the domain of 
runtime.

Very straight forward. Just return the value stored in the token.

### 7.2.2 Evaluating Parentheses
Recursively evaluate the expression inside grouping. `evaluate()` method simply
calls the expression to be evaluated from the interpreter once again.<br>
Like: `return.accept(this)`

### 7.2.3 Evaluating Unary
First evaluate the subexpression passed to the unary operator, and then
evaluate whatever the unary operator is going to do.

Interpreter is doing a **post-order traversal** in order to evaluate unary op.

### 7.2.4 Truthiness and Falsiness
Lox will only return false and nil as false and everything else as true.

## 7.3 Runtime Errors
Must report them! These errors are checked inside the interpreter.<br>
Example: 2 * (3 / -"muffin"). This needs to report at - about wrong expression,
and also at / and then at \*. Need to escape out while reporting errors.

Runtime error should not kill the interpreter. It just needs to report the err.

### 
