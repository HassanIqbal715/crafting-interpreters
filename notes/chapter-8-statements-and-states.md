# Statements and State
Interpreter needs internal state. Where variables can be stored.<br>
Statements don't evaluate to a value. They need to produce a **side effect** to
be useful. Producing output or modifying some state in the interpreter 
(creating variables).

## 8.1 Statements
Extending Lox's grammar with statements.

2 simple kind of statements:
1. **Expression statement**: place an expression where statement is expected.
    Evalue expressions that have side effects. Example: fnction/method call.
2. **Print statement**: evaluate an expression and display the result. Baking
    it right in. No function call. Print is a pre-defined statement.

Program is a collection of statements, so main production is:<br>
```
program     -> statement* EOF;
statement   -> exprStmt | printStmt;
exprStmt    -> expression ";";
printStmt   -> "print" expression ";";
```

### 8.1.1 Statement syntax trees
Both statement and expressions are never allowed together. + only works
on expressions and while loop only works on statements. This means that
Their classes can be separated.

## 8.2 Global Variables
Time to start working on state.

Need 2 new constructs:
1. Variable Declaration.
2. Variable Expression.

### 8.2.1 Variable Syntax
Making a new declaration rule.
```
program -> declaration* EOF;
declaration -> varDecl | statement;
statement -> exprStmt | printStmt;
varDecl -> "var" IDENTIFIER ("=" expression)? ";";
```
Time to also update `primary` expression so that it can support `IDENTIFIER`.

Now since declaration is lower in the precendence, `parse()` method will load
declarations that can be used to either produce statements or variable
declarations.

Reading a variable.
1. if VAR token is matched, go into this.
2. consume identifier.
3. if equal exists after the identifier, extract the expression after it. else
   just initialize the variable with null
4. consume semi colon.
5. return a new statement variable with name and the expression value (if any).

Since variables are also expressions (the 2nd construct), `primary` must also
be able to produce it.

## 8.3 Environments
Where does the variable live in memory? environment.<br>
A data structure that stores variables associated to values.
```
var a = 1; would be a -> 1 in an environment
```
Variables are like keys storing a value. Map can be used to implement this.
- String as key and Literal as value.
- A simple define function can do it.

### Challenges
8.3. I checked how c++ would do it and it printed garbage. Then it got me
wondering how Lox would do it and lox printed 3. Very interesting.
