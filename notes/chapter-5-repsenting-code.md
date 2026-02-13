# Representing Code
Parser: Takes tokens and transforms them into a more complex representation.

## Chapter Objective
- Define that complex representation.
- Formal grammars theory.
- Difference between functional and object oriented programming.
- Some design patterns.
- Metaprogramming.

## Representing Code
as a tree that represents the grammatical structure. Just like the arithmetic tree.<br>
**Formal Language**: set of strings with well-defined rules. These rules determine which strings belong to the language and which do not.<br>
**Context Free Grammars**: Has the capability to define infinite languages. Previously used regular language was finite which means that it can not be used for more complex arbitrary length strings.

### Lexical vs Syntactic grammar
Scanner has whole Lexemes which can be considered as one token. This type of grammar is called lexical grammar. Whereas every character/alphabet counts in syntactic grammar.

Lexical: Alphabet is a character. String is a lexeme or token. Implemented by scanner.<br>
Syntactic: Alphabet is a token. String is an expression. Implemented by the parser.

### Rules for grammar
We cannot list out all the possible combinations for an infinite language. Instead we can write a set of finite rules that can generate valid strings.<br>
**Derivation**: String derived from a grammar's rule.<br>

#### Production
Rules are called productions because they are used to produce strings.
- Each production has a name called its head and a body which tells what it produces.
**Body**: A list of symbols. Contains 2 main types of symbols. Terminals or Nonterminals.<br>
**Terminals**: No continuation. Alphabet. like tokens coming from the scanner (if or 1234).<br>
**Non-terminals**: Head. Another rule.<br>
- **NOTE**: Multiple rules may have the same name. You may pick which rule you want to use when you encounter the respective non-terminal<br>

#### Our CFG notation
head -> non-terminal "terminal" non-terminal non-terminal "terminal";<br>
example:<br>
breakfast -> protein "with" breakfast "on the side";

Enhancing it!<br>
Enhancement 1:<br>
breakfast -> breakfast rule 1 | breakfast rule 2 | breakfast rule 3. // Writing all the rules in one line.

Enhancement 2:<br>
cooked -> "scrambled" | "poached" | "fried";<br>
protein -> cooked "eggs"<br>
here, cooked can be replaced by it's rules<br>
protein -> ( "scrambled" | "poached" | "fried" ) "eggs"

Enhancement 3: Loops 0 or many<br>
crispiness -> "really" "really"\*;

Enhancement 4: Loops atleast once<br>
crispiness -> "really"+;

Enhancement 5: None or once<br>
breakfast -> protein ( "with" breakfast "on the side" )?;

### Syntax Tree
Assuming we have now formulated an expression grammar that checks validity (syntax), our data structure will make a tree out of it called a syntax tree.<br>

- In a parse tree, every single grammar production becomes a node in the tree.
- AST omits productions that are not needed later on.

#### Dioriented Objects
Here, our expression class is just a bag of data. Nothing more. Why?<br>
These exist to help interpreter and parser communicate with each other<br>

#### Metaprogramming the tree
Metaprogramming is technique where other parts of the program are treated as data.<br>

#### Why do we need it here?
Writing a constructor, initializer etc is very tedious. Can be replaced by this technique.<br>
We will write a script that generates Expr.java for us. It will contain name and fields for each tree. Just like we want it to. It prints out java code needed to define class with that name and state.<br>

### Working with trees
#### The expression problem
Classes are made so that if a new type is added, the existing methods/code is not changed, but if a new method is added, then every single class is changed to accomodate that.

Functions are made so that if a new method is added, the existing methods/code are not changed, but if anew type is added, then every single method is changed to accomodate that.

#### The visitor pattern
A type of code design pattern.

- Allows us to approximate function style in OOP.
- Can easily add new methods with this.
- No need to touch the existing types.

Does that by adding a level of indirection to the code.

##### implementing it
1. Define a separate interface. It contains a separate visit function for each class. In expression would be 4 for each type of expression (literal, unary, binary, grouping).
2. Now how to indirect a given input to its correct class/type? Polymorphism. Each class implements an accept function. It takes visitor of type interface as a parameter, and calls the specific function defined for it inside the visitor interface.

##### My understanding
1. user calls something like UnaryOperatorVariable.accept(someVisitorFunctionClass);
2. In accept it will call return someVisitorFunctionClass.visit(this);

- The thing is that every single operation/function has its own class. 
- A visitor class that has a defined function for each and every type in it. 
- That type of function is used as an argument for our accept functions.
- The respective function is called with the caller of accept as the argument (this).

### Pretty Printer
Shows the state of our syntax tree at a given time.<br>
- Useful for debugging the parser and the interpreter.<br>
- We want a string that explicitly shows the nesting structure of the parse tree.<br>

#### Method
Explicitly put each expression inside parentheses. All of the sub tokens and expressions are also contained in that.
1. Binary = (leftExpression operator rightExpression)
2. Literal = Just their value
3. Unary = (operator rightExpression)
4. Grouping = (group expression)

### Challenges
#### 5.1 Language
expr -> IDENTIFIER<br>
expr -> NUMBER<br>
expr -> expr C<br>
A    -> AA<br>
A    -> "," expr<br>
A    -> ε<br>
B    -> expr A<br>
C    -> "(" B ")"<br>
C    -> "." IDENTIFIER<br>
C    -> CC<br>

I think the language is like can have a list of elements and identifiers inside of parentheses. (Even more parenthesized numbers and identifiers).<br>
And also decimal numbers along with values/methods access of an identifier/listofthings.<br>

#### 5.3
Done! :)