# Chapter 11 Resolving and Binding
## Static Scope
**Semantic Analysis**: extract info without running the code. Things that show just by reading the code.

**Rules**: A variable usage refers to the preceding declaration with the same 
           name in the innermost scope that encloses the expression where the
           variable is used.
- Variable usage = assignment + expression.
- Preceding = appearing before in text in the program. Sometimes execution just
    because it appears before may not be the case (dynamic temporal execution).
    Sometimes it may be which is (static textual ordering).
- Innermost = Shadowing may occur.

### Problem with closures
Currently the environment exists as a hashtable that changes over time. A 
function has closure to that live hashtable. That mutable object that may 
change over time. That results in the function using updated values that don't
make sense. 

**Solution**: Instead of a live mutable object, the closure should receive a
frozen snapshot of the environment at the time of function declaration.

### Solving the problem with persistent data structure
Can never be directly modified. Any change means that a new object is created
with the original data + the new data. Original object is left unmodified.
Now original is reassigned with this new 

## Semantic Analysis
Currently when interpreting, when we encounter a variable, we track it down
through the environments every single time. If a loop runs 1000 times, then the
variable is tracked down/RESOLVED 1000 times. Big problem!

**Solution**: Semantic analysis. Read the whole code once, see all the
    declarations once. Currently, the code only sees that it's a declaration,
    now it will know which declaration it is.
- Need to check how many hops it takes between environments to find the correct
  scope of declaration.
- Where do that? In the parser. Another technique is Resolver.


### Resolver
Mini-interpreter
- No side-effects: Only visits, does not execute.
- No control flow. Each loop is visited only once. Both branches are visited in
  if statement. Logic operators are not short-circuited.
  
We are only interested in 4 types of expressions and statements.
- Block statement = New scope.
- Function statement = New scope for the body and binds the new parameters.
- Variable declaration = Adds new variable to the current scope.
- Variables and Assignments need to have their variables resolved.

## Coding stuffs
Our resolver is good for telling how many scopes away is the variable being
used.

Create a stack with hashmaps<String, Boolean>. Store all the local scopes
inside the new stack. With each scope creation, a hashmap is pushed.

An error that we need to consider:
```
var a = 2;
{
    var a = a;
}
```
The inner variable is technically referring to itself which is wrong.<br>
How handle? Do binding in 2 steps. Declaring first with bool false and then 
defining with bool true.

When resolving a local variable, we go from the innermost scope to the outside,
checking all the scopes for the variable that we want. When found we return the
scope index. Insane stuffs right? 0 means closest.

Since functions can call themselves for recursion, they are defined before
getting resolved. Unlike variables.
