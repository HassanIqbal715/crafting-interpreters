# Chapter 9
Making our language turing complete.

So far our code runs the length of its source code (Can't loop back or smth).

## 9.1 Turing Machine
Can compute all computable functions. The rest are noncomputable functions
like a function that just returns the truth value of a given statement.

Requirements for a turing machine/turing complete language:
1. arithmetic.
2. control flow.
3. allocate and use arbitrary amounts of memory.

## 9.2 Conditional execution
2 types of control flows.
1. conditional/branching control flow. Used to skip code lol.
2. looping control flow. Execute code more than once. It has conditional logic
   to prevent infinite looping of code.

### If branching
2 types of branchings:
1. `if` statement. Conditionally execute statements.
2. `?:` ternary operator. Conditionally execute expressions.

**language update**:
```
statement -> ifStmt; (added this)
ifStmt -> "if" "(" expression ")" statement ("else" statement)?;
```
Basic-most if statement that executes just one statement.

Big problem! Consider this.
```
if (first)
    if (second)
        whenTrue();
    else
        whenFalse();
```

Does that `else` belong to the first `if` or the second?<br>
Second! `else` always belongs to the nearest `if` that precedes it (in most
languages).

Stages to writing If!
1. Make sure IF token exists.
2. Parse! Like this -> Match IF, consume `(`, read the condition, consume `)`,
   read the "then branch", Match `else` if else exists, read the "else branch"
   else keep it null, and finally return statement with condition, "then
   branch" and "else branch".
3. Interpret it! Like this -> evaluate the condition, return "then branch" if
   true, else return "else branch".

### While loop
**language update**:
```
statement -> whileStmt; (added this)
whileStmt -> "while" "("  expression ")" body;
```

Stages to adding while!
1. Make sure WHILE token exsits.
2. Parse! Like this -> Match WHILE, consume `(`, read the condition, consume
   `)`, read the body. return a new while statement with condition and body.
3. Interpret it! Like this -> if the condition is true, just execute the
   body in a loop. That's it.

### For loop
**language update**:
```
statement -> forStmt; (added this)
forStmt -> "for" "(" ( varDecl | exprStmt | ";" )
            expression? ";"
            expression? ")" statement;
```

1. First clause is *initializer*. Executed once before starting.
2. Second clause is *condition*. Executed to check isTruthy before every it.
3. Third clause is *increment*. Executed to change value of our condition var.

**Synctactic Sugar**: Lox doesn't need for loop. All work can be done using
while loop already, but this just makes some common code patters more pleasant
to write. :3

**Desugaring**: Front end takes code using syntax sugar (for loop) and just
uses primitive form in the backend (execute while loop in the backend lol).

- Desugaring is why a new type of tree node is not required here :3.

        
