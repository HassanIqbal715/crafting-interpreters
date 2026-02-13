# Parser
Given a series of tokens, map those tokens to terminals in the grammar (CFG) and find out which rules could have derived it.

## 6.1 Ambiguity
Its totally possible to create an ambiguous grammar and then have different productions lead to the same string. Ambiguity means that the parser misunderstood the user's code.

an expression such as 1 + 2 * 3 may be generated as either
(1 + 2) * 3 or 1 + (2 * 3) depending on how the binary expression generates it. Take for example the first expression.
1. expression -> binary1
2. binary1.left -> binary2
3. binary2.left -> 1
4. binary2.operator -> +
5. binary2.right -> 2
6. binary1.operator -> *
7. binary1.right -> 3

### Solution!
**Precedence**: Determines which operator is evaluated first. Will tell to evaluate * before -.

**Associativity**: Tells which operator is evaluated first in a series of the same operator. Left associative (Left-to-Right). 5 - 3 - 1 is equivalent to (5-3) -1 in case of left associative.

Assignment(=) is right associative. a = b = c is equivalent to a = (b = c).

### Applying Precedence
Making a different rule for every single precedence.<br>
Equality < Comparison < Term < Factor < Unary < Primary (literals and parentheses)

```
primary → NUMBER | STRING | "true" | "false" | "nil" | "(" expression ")" ;
factor -> factor ( "/" | "*" ) unary | unary ;
```
For factor, putting the factor on the left side and unary on the right side makes it left-associative/left-recursive.

#### Problem
our parser has a problem with left recursive productions, so we will use flat sequence with no left-recursion.

```
factor -> unary (( "/" | "*" ) unary)*;
```

## Recursive Descent
the most basic parsing technique.

- top-down parser. Why? it starts from the outermost grammar rule and works it's way down the subexpressions. Lower to higher precedence.
- predictive parser. It looks ahead to decide how to parse tokens.
- Each rule becomes a function.

Rules functions are like so:<br>
**Grammar Notation**   **Code Representation**
- Terminal              Code to match and consume
- Nonterminal           Function call
- |                     if or switch statement
- * or +                while or for loop
- ?                     if statement

Why left recursion is bad?<br>
A caller is expected to receive a tree. If it calls itself, it will be stuck in an infinite recursion and die.

### How this method works
Example for equality production
1. left node of tree is comparison().
2. middle is operator.
3. right is comparison().
4. when creating a binary stump now, this left, operator and right are assigned well.
5. The next iteration. Let's say another == is matched now. This is where the magic happens. our left node is assigned to our previously created binary tree.
6. operator now is a new operator read (==).
7. right is another comparison() call.

The previous stump is now at the left of the new tree. This keeps up as long as equality operators exist. Tree is forever left-associative.

## 6.3 Syntax Errors
Parser has 2 jobs.
1. valid tokens -> syntax tree.
2. invalid token -> error.

### What to do when parser runs into error
1. Detect and report the error.
2. Avoid crashing or hanging.

### Decent parser
1. Be fast.
2. Report errors.
3. Minimize cascaded code. Parser may give ghost errors once one error is found. Ghosts are part of a parser's own confusion.

### Error recovery
The way parser responds to an error and keeps going back to look for later errors.

#### Panic mode
One of the best recovery techniques.<br>
When parser finds one error, it enters panic mode.<br>
Before it can get back to parsing, it needs to find the next token that matches. This helps get back on track (recover) and it is called **Synchronisation**.

**How synchronise?**
1. Pick a rule that will mark the sync point.
2. Parser will jump out of the nested productions till it reaches the sync point.
3. Syncs the tokens by discarding all the ones that don't belong to the sync point production until it finds one that matches the sync point produciton.

We will be using java's call stack to throw exceptions until sync point catches it.
